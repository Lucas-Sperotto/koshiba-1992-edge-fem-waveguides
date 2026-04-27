#include "koshiba/fem/global_assembly.hpp"

#include <vector>

#include "koshiba/fem/local_integrals.hpp"

namespace koshiba::fem {
namespace {

using Triplet = Eigen::Triplet<double>;

void add_edge_edge_terms(std::vector<Triplet>& triplets,
                         const mesh::ElementEdgeConnectivity& connectivity,
                         const Eigen::Matrix3d& local) {
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto local_row = static_cast<std::size_t>(row);
            const auto local_col = static_cast<std::size_t>(col);
            const double sign =
                static_cast<double>(connectivity.signs[local_row] * connectivity.signs[local_col]);
            triplets.emplace_back(
                static_cast<Eigen::Index>(connectivity.edge_ids[local_row]),
                static_cast<Eigen::Index>(connectivity.edge_ids[local_col]),
                sign * local(row, col));
        }
    }
}

void add_edge_node_terms(std::vector<Triplet>& triplets,
                         const mesh::Mesh& mesh,
                         const mesh::TriangleElement& element,
                         const mesh::ElementEdgeConnectivity& connectivity,
                         const Eigen::Matrix3d& local) {
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto local_row = static_cast<std::size_t>(row);
            const auto local_col = static_cast<std::size_t>(col);
            const double sign = static_cast<double>(connectivity.signs[local_row]);
            triplets.emplace_back(
                static_cast<Eigen::Index>(connectivity.edge_ids[local_row]),
                static_cast<Eigen::Index>(mesh.node_index(element.node_ids[local_col])),
                sign * local(row, col));
        }
    }
}

void add_node_node_terms(std::vector<Triplet>& triplets,
                         const mesh::Mesh& mesh,
                         const mesh::TriangleElement& element,
                         const Eigen::Matrix3d& local) {
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto local_row = static_cast<std::size_t>(row);
            const auto local_col = static_cast<std::size_t>(col);
            triplets.emplace_back(
                static_cast<Eigen::Index>(mesh.node_index(element.node_ids[local_row])),
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

}  // namespace

GlobalAssemblyBlocks assemble_geometric_blocks(const mesh::Mesh& mesh) {
    const Eigen::Index edge_count = static_cast<Eigen::Index>(mesh.edges().size());
    const Eigen::Index node_count = static_cast<Eigen::Index>(mesh.nodes().size());

    std::vector<Triplet> ktt_curl;
    std::vector<Triplet> ktz_u_nx;
    std::vector<Triplet> ktz_v_ny;
    std::vector<Triplet> kzz_nx_nx;
    std::vector<Triplet> kzz_ny_ny;
    std::vector<Triplet> mtt_uu;
    std::vector<Triplet> mtt_vv;
    std::vector<Triplet> mzz_nn;

    for (std::size_t element_index = 0; element_index < mesh.elements().size(); ++element_index) {
        const auto triangle = mesh.triangle(element_index);
        const auto local = compute_local_integrals(triangle);
        const auto& element = mesh.elements()[element_index];
        const auto& connectivity = mesh.element_edges()[element_index];

        add_edge_edge_terms(ktt_curl, connectivity, 4.0 * local.a3_derivative);
        add_edge_edge_terms(mtt_uu, connectivity, local.a1_uu);
        add_edge_edge_terms(mtt_vv, connectivity, local.a2_vv);
        add_edge_node_terms(ktz_u_nx, mesh, element, connectivity, local.a4_u_nx);
        add_edge_node_terms(ktz_v_ny, mesh, element, connectivity, local.a5_v_ny);
        add_node_node_terms(kzz_nx_nx, mesh, element, local.a7_nx_nx);
        add_node_node_terms(kzz_ny_ny, mesh, element, local.a8_ny_ny);
        add_node_node_terms(mzz_nn, mesh, element, local.a6_nn);
    }

    return {
        make_sparse(edge_count, edge_count, ktt_curl),
        make_sparse(edge_count, node_count, ktz_u_nx),
        make_sparse(edge_count, node_count, ktz_v_ny),
        make_sparse(node_count, node_count, kzz_nx_nx),
        make_sparse(node_count, node_count, kzz_ny_ny),
        make_sparse(edge_count, edge_count, mtt_uu),
        make_sparse(edge_count, edge_count, mtt_vv),
        make_sparse(node_count, node_count, mzz_nn),
    };
}

}  // namespace koshiba::fem
