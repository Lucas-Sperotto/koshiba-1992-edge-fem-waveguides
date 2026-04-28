#pragma once

#include <Eigen/Sparse>

#include "koshiba/mesh/mesh.hpp"

namespace koshiba::fem {

/**
 * @brief Holds the global geometric blocks assembled from local integrals.
 *
 * These matrices are purely geometric and do not include material properties (p, q)
 * or physical constants (k0, beta). They are combined later to form the final
 * FEM matrices according to the formulation (e.g., Eqs. 29, 32).
 */
struct GlobalAssemblyBlocks {
    // Term from the expansion of (curl_t phi_t) . (curl_t phi_t).
    // It stores 4 * integral({U_y}{U_y}^T), which is positive.
    // In Eq. (29a), this block is multiplied by p_z and added.
    // In Eq. (32a), this block is multiplied by p_z and subtracted.
    Eigen::SparseMatrix<double> ktt_curl;

    // Term for K_tz, coupling {U} with {N_x}. In K_tz, it's multiplied by p_y.
    Eigen::SparseMatrix<double> ktz_u_nx;
    // Term for K_tz, coupling {V} with {N_y}. In K_tz, it's multiplied by p_x.
    Eigen::SparseMatrix<double> ktz_v_ny;

    // Term for K_zz, from {N_x}{N_x}^T. In K_zz, it's multiplied by p_y.
    Eigen::SparseMatrix<double> kzz_nx_nx;
    // Term for K_zz, from {N_y}{N_y}^T. In K_zz, it's multiplied by p_x.
    Eigen::SparseMatrix<double> kzz_ny_ny;

    // Mass matrix term for phi_x component, from {U}{U}^T.
    Eigen::SparseMatrix<double> mtt_uu;
    // Mass matrix term for phi_y component, from {V}{V}^T.
    Eigen::SparseMatrix<double> mtt_vv;
    // Mass matrix term for phi_z component, from {N}{N}^T.
    Eigen::SparseMatrix<double> mzz_nn;
};

/**
 * @brief Assembles the purely geometric sparse matrix blocks.
 * @param mesh The mesh to assemble on.
 * @return A struct containing all assembled geometric blocks.
 */
GlobalAssemblyBlocks assemble_geometric_blocks(const mesh::Mesh& mesh);

}  // namespace koshiba::fem
