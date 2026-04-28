#include "koshiba/fem/global_assembly.hpp"

#include <vector>
#include <map>

#include "koshiba/fem/local_integrals.hpp"

namespace koshiba::fem {
namespace {

using Triplet = Eigen::Triplet<double>;

}  // namespace

GlobalAssemblyBlocks assemble_geometric_blocks(const mesh::Mesh& mesh) {
    const Eigen::Index edge_count = static_cast<Eigen::Index>(mesh.edges().size());
    const Eigen::Index node_count = static_cast<Eigen::Index>(mesh.nodes().size());
    
    std::vector<Triplet> ktt_curl_triplets;
    std::vector<Triplet> ktz_u_nx_triplets;
    std::vector<Triplet> ktz_v_ny_triplets;
    std::vector<Triplet> kzz_nx_nx_triplets;
    std::vector<Triplet> kzz_ny_ny_triplets;
    std::vector<Triplet> mtt_uu_triplets;
    std::vector<Triplet> mtt_vv_triplets;
    std::vector<Triplet> mzz_nn_triplets;
    
    for (const auto& triangle : mesh.get_triangles()) {
        const auto& local_integrals = triangle.get_local_integrals();
        
        // This loop assumes that the triangle has a counter-clockwise orientation
        // (positive area). This is a precondition for the local integral
        // formulas to be correct. The mesh::Mesh class is responsible for
        // enforcing this by rejecting clockwise or degenerate triangles upon creation.
        
        const auto& edge_indices = triangle.get_edge_indices();
        const auto& node_indices = triangle.get_node_indices();
        const auto& signs = triangle.get_edge_signs();
        
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                const int global_row = edge_indices[i];
                const int global_col = edge_indices[j];
                const double val = local_integrals.a3(i, j);
                
                // This block stores 4 * integral({U_y}{U_y}^T), which is positive.
                // It corresponds to the expansion of the transverse curl-curl term:
                // integral( (-{U_y}+{V_x}) * (-{U_y}+{V_x})^T ).
                // In Eq. (29a), this term is added (multiplied by p_z).
                // In Eq. (32a), this term is subtracted (multiplied by p_z).
                ktt_curl_triplets.emplace_back(global_row, global_col, 4.0 * val * signs[i] * signs[j]);
                mtt_uu_triplets.emplace_back(global_row, global_col, local_integrals.a1(i, j) * signs[i] * signs[j]);
                mtt_vv_triplets.emplace_back(global_row, global_col, local_integrals.a2(i, j) * signs[i] * signs[j]);
            }
        }
        
        for (int i = 0; i < 3; ++i) { // edge
            for (int j = 0; j < 3; ++j) { // node
                const int global_row = edge_indices[i];
                const int global_col = node_indices[j];
                ktz_u_nx_triplets.emplace_back(global_row, global_col, local_integrals.a4(i, j) * signs[i]);
                ktz_v_ny_triplets.emplace_back(global_row, global_col, local_integrals.a5(i, j) * signs[i]);
            }
        }
        
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                const int global_row = node_indices[i];
                const int global_col = node_indices[j];
                kzz_nx_nx_triplets.emplace_back(global_row, global_col, local_integrals.a7(i, j));
                kzz_ny_ny_triplets.emplace_back(global_row, global_col, local_integrals.a8(i, j));
                mzz_nn_triplets.emplace_back(global_row, global_col, local_integrals.a6(i, j));
            }
        }
    }

    GlobalAssemblyBlocks blocks;
    blocks.ktt_curl.setFromTriplets(ktt_curl_triplets.begin(), ktt_curl_triplets.end());
    blocks.ktz_u_nx.setFromTriplets(ktz_u_nx_triplets.begin(), ktz_u_nx_triplets.end());
    blocks.ktz_v_ny.setFromTriplets(ktz_v_ny_triplets.begin(), ktz_v_ny_triplets.end());
    blocks.kzz_nx_nx.setFromTriplets(kzz_nx_nx_triplets.begin(), kzz_nx_nx_triplets.end());
    blocks.kzz_ny_ny.setFromTriplets(kzz_ny_ny_triplets.begin(), kzz_ny_ny_triplets.end());
    blocks.mtt_uu.setFromTriplets(mtt_uu_triplets.begin(), mtt_uu_triplets.end());
    blocks.mtt_vv.setFromTriplets(mtt_vv_triplets.begin(), mtt_vv_triplets.end());
    blocks.mzz_nn.setFromTriplets(mzz_nn_triplets.begin(), mzz_nn_triplets.end());
    
    return blocks;
}

}  // namespace koshiba::fem
