#include "koshiba/fem/beta_solver.hpp"

#include "koshiba/algebra/reduced_eigenproblem.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <set>
#include <stdexcept>

namespace koshiba::fem {
namespace {

bool contains(const std::vector<int>& values, int value) {
    return std::find(values.begin(), values.end(), value) != values.end();
}

void append_unique(std::vector<int>& target, const std::vector<int>& values) {
    for (int value : values) {
        if (!contains(target, value)) {
            target.push_back(value);
        }
    }
}

std::vector<Eigen::Index> free_edge_indices(const mesh::Mesh& mesh,
                                            const BoundaryDofConstraints& constraints) {
    std::vector<Eigen::Index> indices;
    for (std::size_t edge_index = 0; edge_index < mesh.edges().size(); ++edge_index) {
        const auto& edge = mesh.edges()[edge_index];
        if (!contains(constraints.edge_physical_tags, edge.physical_tag())) {
            indices.push_back(static_cast<Eigen::Index>(edge_index));
        }
    }
    return indices;
}

std::vector<Eigen::Index> free_node_indices(const mesh::Mesh& mesh,
                                            const BoundaryDofConstraints& constraints) {
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

    std::vector<Eigen::Index> indices;
    for (std::size_t node_index = 0; node_index < mesh.nodes().size(); ++node_index) {
        if (constrained_node_ids.count(mesh.nodes()[node_index].id) == 0) {
            indices.push_back(static_cast<Eigen::Index>(node_index));
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

BetaMatrices assemble_beta_matrices(const GlobalAssemblyBlocks& blocks,
                                    const physics::DiagonalCoefficients& coefficients,
                                    double k0) {
    const double k0_squared = k0 * k0;
    return {
        k0_squared * (coefficients.q_x * blocks.mtt_uu +
                      coefficients.q_y * blocks.mtt_vv) -
            coefficients.p_z * blocks.ktt_curl,
        coefficients.p_y * blocks.ktz_u_nx +
            coefficients.p_x * blocks.ktz_v_ny,
        k0_squared * coefficients.q_z * blocks.mzz_nn -
            coefficients.p_y * blocks.kzz_nx_nx -
            coefficients.p_x * blocks.kzz_ny_ny,
        coefficients.p_y * blocks.mtt_uu +
            coefficients.p_x * blocks.mtt_vv,
    };
}

BoundaryDofConstraints essential_boundary_constraints(
    physics::FieldKind field_kind,
    const std::vector<PhysicalBoundaryCondition>& boundary_conditions) {
    BoundaryDofConstraints constraints;
    for (const auto& condition : boundary_conditions) {
        const bool constrains_solved_field =
            (field_kind == physics::FieldKind::Electric &&
             condition.kind == BoundaryConditionKind::PEC) ||
            (field_kind == physics::FieldKind::Magnetic &&
             condition.kind == BoundaryConditionKind::PMC);
        if (!constrains_solved_field) {
            continue;
        }

        append_unique(constraints.edge_physical_tags, condition.physical_tags);
        append_unique(constraints.node_physical_tags, condition.physical_tags);
    }

    return constraints;
}

BetaSolveResult solve_beta_modes(const mesh::Mesh& mesh,
                                 const physics::DiagonalMaterial& material,
                                 physics::FieldKind field_kind,
                                 double k0,
                                 const BetaSolverOptions& options) {
    if (k0 <= 0.0) {
        throw std::invalid_argument("k0 must be positive");
    }

    const auto coefficients = material.coefficients(field_kind);
    const auto geometric_blocks = assemble_geometric_blocks(mesh);
    const auto beta_matrices = assemble_beta_matrices(geometric_blocks, coefficients, k0);

    const auto free_edges = free_edge_indices(mesh, options.constraints);
    const auto free_nodes = free_node_indices(mesh, options.constraints);
    if (free_edges.empty() || free_nodes.empty()) {
        throw std::invalid_argument("beta solve requires at least one free edge and node dof");
    }

    const Eigen::MatrixXd ktt =
        dense_submatrix(beta_matrices.ktt_beta, free_edges, free_edges);
    const Eigen::MatrixXd ktz =
        dense_submatrix(beta_matrices.ktz_beta, free_edges, free_nodes);
    const Eigen::MatrixXd kzz =
        dense_submatrix(beta_matrices.kzz_beta, free_nodes, free_nodes);
    const Eigen::MatrixXd mtt =
        dense_submatrix(beta_matrices.mtt_beta, free_edges, free_edges);

    const auto beta_result =
        koshiba::algebra::solve_beta_squared_self_adjoint(ktt, ktz, kzz, mtt);

    BetaSolveResult result;
    result.transverse_eigenvectors = beta_result.eigenvectors;
    result.effective_mass = beta_result.effective_mass;
    result.field_kind = field_kind;
    result.k0 = k0;
    result.free_edge_dofs = free_edges.size();
    result.free_node_dofs = free_nodes.size();

    const std::size_t max_modes =
        std::min<std::size_t>(options.requested_modes,
                              static_cast<std::size_t>(beta_result.beta2.size()));
    result.modes.reserve(max_modes);
    for (std::size_t mode = 0; mode < max_modes; ++mode) {
        const double beta2 = beta_result.beta2(static_cast<Eigen::Index>(mode));
        const bool finite = std::isfinite(beta2);
        const bool positive = beta2 > options.positive_tolerance;
        result.modes.push_back({
            mode,
            beta2,
            positive ? std::sqrt(beta2) : std::numeric_limits<double>::quiet_NaN(),
            finite ? (positive ? "propagating" : "non_positive_beta2") : "non_finite",
        });
    }

    return result;
}

}  // namespace koshiba::fem
