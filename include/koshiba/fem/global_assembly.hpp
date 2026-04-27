#pragma once

#include <Eigen/Sparse>

#include "koshiba/mesh/mesh.hpp"

namespace koshiba::fem {

struct GlobalAssemblyBlocks {
    Eigen::SparseMatrix<double> ktt_curl;
    Eigen::SparseMatrix<double> ktz_u_nx;
    Eigen::SparseMatrix<double> ktz_v_ny;
    Eigen::SparseMatrix<double> kzz_nx_nx;
    Eigen::SparseMatrix<double> kzz_ny_ny;
    Eigen::SparseMatrix<double> mtt_uu;
    Eigen::SparseMatrix<double> mtt_vv;
    Eigen::SparseMatrix<double> mzz_nn;
};

GlobalAssemblyBlocks assemble_geometric_blocks(const mesh::Mesh& mesh);

}  // namespace koshiba::fem
