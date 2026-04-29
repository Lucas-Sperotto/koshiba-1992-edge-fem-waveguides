#include "koshiba/fem/direct_frequency_solver.hpp"

#include "koshiba/fem/local_integrals.hpp"
#include "koshiba/physics/normalization.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <stdexcept>

namespace koshiba::fem {
namespace {

using Triplet = Eigen::Triplet<double>;

bool contains(const std::vector<int>& values, int value) {
    return std::find(values.begin(), values.end(), value) != values.end();
}

void add_edge_edge_terms(std::vector<Triplet>& triplets,
                         const mesh::ElementEdgeConnectivity& connectivity,
                         const Eigen::Matrix3d& local) {
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto local_row = static_cast<std::size_t>(row);
            const auto local_col = static_cast<std::size_t>(col);
            const double sign =
                static_cast<double>(connectivity.signs[local_row] *
                                    connectivity.signs[local_col]);
            triplets.emplace_back(
                static_cast<Eigen::Index>(connectivity.edge_ids[local_row]),
                static_cast<Eigen::Index>(connectivity.edge_ids[local_col]),
                sign * local(row, col));
        }
    }
}

void add_symmetric_edge_node_terms(std::vector<Triplet>& triplets,
                                   const mesh::Mesh& mesh,
                                   const mesh::TriangleElement& element,
                                   const mesh::ElementEdgeConnectivity& connectivity,
                                   const Eigen::Matrix3d& local,
                                   Eigen::Index node_offset) {
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto local_row = static_cast<std::size_t>(row);
            const auto local_col = static_cast<std::size_t>(col);
            const double sign = static_cast<double>(connectivity.signs[local_row]);
            const Eigen::Index edge_dof =
                static_cast<Eigen::Index>(connectivity.edge_ids[local_row]);
            const Eigen::Index node_dof =
                node_offset +
                static_cast<Eigen::Index>(mesh.node_index(element.node_ids[local_col]));
            const double value = sign * local(row, col);
            triplets.emplace_back(edge_dof, node_dof, value);
            triplets.emplace_back(node_dof, edge_dof, value);
        }
    }
}

void add_node_node_terms(std::vector<Triplet>& triplets,
                         const mesh::Mesh& mesh,
                         const mesh::TriangleElement& element,
                         const Eigen::Matrix3d& local,
                         Eigen::Index node_offset) {
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto local_row = static_cast<std::size_t>(row);
            const auto local_col = static_cast<std::size_t>(col);
            triplets.emplace_back(
                node_offset +
                    static_cast<Eigen::Index>(mesh.node_index(element.node_ids[local_row])),
                node_offset +
                    static_cast<Eigen::Index>(mesh.node_index(element.node_ids[local_col])),
                local(row, col));
        }
    }
}

Eigen::SparseMatrix<double> make_sparse(Eigen::Index rows,
                                        Eigen::Index cols,
                                        const std::vector<Triplet>& triplets) {
    Eigen::SparseMatrix<double> matrix(rows, cols);
    matrix.setFromTriplets(triplets.begin(), triplets.end());
    return matrix;
}

std::vector<Eigen::Index> free_full_indices(const mesh::Mesh& mesh,
                                            const BoundaryDofConstraints& constraints) {
    std::vector<Eigen::Index> indices;
    for (std::size_t edge_index = 0; edge_index < mesh.edges().size(); ++edge_index) {
        const auto& edge = mesh.edges()[edge_index];
        if (!contains(constraints.edge_physical_tags, edge.physical_tag())) {
            indices.push_back(static_cast<Eigen::Index>(edge_index));
        }
    }

    std::set<std::size_t> constrained_node_ids;
    if (!constraints.node_physical_tags.empty() || constraints.constrain_nodes_touching_edges) {
        for (const auto& edge : mesh.edges()) {
            const bool node_tag_match =
                contains(constraints.node_physical_tags, edge.physical_tag());
            const bool legacy_edge_match =
                constraints.constrain_nodes_touching_edges &&
                contains(constraints.edge_physical_tags, edge.physical_tag());
            if (node_tag_match || legacy_edge_match) {
                constrained_node_ids.insert(edge.first_node_id());
                constrained_node_ids.insert(edge.second_node_id());
            }
        }
    }

    const Eigen::Index node_offset = static_cast<Eigen::Index>(mesh.edges().size());
    for (std::size_t node_index = 0; node_index < mesh.nodes().size(); ++node_index) {
        if (constrained_node_ids.count(mesh.nodes()[node_index].id) == 0) {
            indices.push_back(node_offset + static_cast<Eigen::Index>(node_index));
        }
    }
    return indices;
}

Eigen::MatrixXd dense_submatrix(const Eigen::SparseMatrix<double>& matrix,
                                const std::vector<Eigen::Index>& rows,
                                const std::vector<Eigen::Index>& cols) {
    Eigen::MatrixXd dense(rows.size(), cols.size());
    for (Eigen::Index row = 0; row < dense.rows(); ++row) {
        for (Eigen::Index col = 0; col < dense.cols(); ++col) {
            dense(row, col) = matrix.coeff(rows[static_cast<std::size_t>(row)],
                                           cols[static_cast<std::size_t>(col)]);
        }
    }
    return dense;
}

}  // namespace

DirectFrequencyMatrices assemble_direct_frequency_matrices(
    const mesh::Mesh& mesh,
    const physics::MaterialMap& materials,
    physics::FieldKind field_kind,
    double beta) {
    if (beta < 0.0) {
        throw std::invalid_argument("beta must be non-negative");
    }

    const Eigen::Index edge_count = static_cast<Eigen::Index>(mesh.edges().size());
    const Eigen::Index node_count = static_cast<Eigen::Index>(mesh.nodes().size());
    const Eigen::Index total_dofs = edge_count + node_count;
    const double beta_squared = beta * beta;

    std::vector<Triplet> stiffness;
    std::vector<Triplet> mass;

    for (std::size_t element_index = 0; element_index < mesh.elements().size(); ++element_index) {
        const auto& element = mesh.elements()[element_index];
        const auto coefficients = materials.coefficients_for(element.physical_tag, field_kind);
        const auto triangle = mesh.triangle(element_index);
        const auto local = compute_local_integrals(triangle);
        const auto& connectivity = mesh.element_edges()[element_index];

        add_edge_edge_terms(
            stiffness,
            connectivity,
            beta_squared * (coefficients.p_y * local.a1_uu +
                            coefficients.p_x * local.a2_vv) +
                coefficients.p_z * (4.0 * local.a3_derivative));
        add_symmetric_edge_node_terms(
            stiffness,
            mesh,
            element,
            connectivity,
            beta * (coefficients.p_y * local.a4_u_nx +
                    coefficients.p_x * local.a5_v_ny),
            edge_count);
        add_node_node_terms(
            stiffness,
            mesh,
            element,
            coefficients.p_y * local.a7_nx_nx +
                coefficients.p_x * local.a8_ny_ny,
            edge_count);

        add_edge_edge_terms(
            mass,
            connectivity,
            coefficients.q_x * local.a1_uu +
                coefficients.q_y * local.a2_vv);
        add_node_node_terms(
            mass,
            mesh,
            element,
            coefficients.q_z * local.a6_nn,
            edge_count);
    }

    return {
        make_sparse(total_dofs, total_dofs, stiffness),
        make_sparse(total_dofs, total_dofs, mass),
    };
}

DirectFrequencyResult solve_direct_frequency_modes(
    const mesh::Mesh& mesh,
    const physics::MaterialMap& materials,
    physics::FieldKind field_kind,
    double beta,
    const BetaSolverOptions& options) {
    const auto matrices = assemble_direct_frequency_matrices(mesh, materials, field_kind, beta);
    const auto free_dofs = free_full_indices(mesh, options.constraints);
    if (free_dofs.empty()) {
        throw std::invalid_argument("direct frequency solve requires at least one free dof");
    }

    const Eigen::MatrixXd stiffness =
        dense_submatrix(matrices.stiffness, free_dofs, free_dofs);
    const Eigen::MatrixXd mass =
        dense_submatrix(matrices.mass, free_dofs, free_dofs);

    Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(stiffness, mass);
    if (eigensolver.info() != Eigen::Success) {
        throw std::runtime_error("direct frequency generalized eigenproblem failed");
    }

    DirectFrequencyResult result;
    result.eigenvectors = eigensolver.eigenvectors();
    result.field_kind = field_kind;
    result.beta = beta;
    result.free_dofs = free_dofs.size();

    const auto eigenvalues = eigensolver.eigenvalues();
    const std::size_t max_modes =
        std::min<std::size_t>(options.requested_modes,
                              static_cast<std::size_t>(eigenvalues.size()));
    result.modes.reserve(max_modes);
    for (std::size_t mode = 0; mode < max_modes; ++mode) {
        const double k0_squared = eigenvalues(static_cast<Eigen::Index>(mode));
        const bool finite = std::isfinite(k0_squared);
        const bool positive = k0_squared > options.positive_tolerance;
        const double k0 = positive ? std::sqrt(k0_squared) :
                                    std::numeric_limits<double>::quiet_NaN();
        result.modes.push_back({
            mode,
            k0_squared,
            k0,
            positive ? physics::frequency_ghz_from_k0(k0) :
                       std::numeric_limits<double>::quiet_NaN(),
            finite ? (positive ? "propagating" : "non_positive_k0_squared") : "non_finite",
        });
    }

    return result;
}

}  // namespace koshiba::fem
