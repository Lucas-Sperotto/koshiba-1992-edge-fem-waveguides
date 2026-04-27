#pragma once

#include <Eigen/Dense>

#include "koshiba/mesh/triangle.hpp"

namespace koshiba::fem {

struct LocalIntegrals {
    Eigen::Matrix3d a1_uu;
    Eigen::Matrix3d a2_vv;
    Eigen::Matrix3d a3_derivative;
    Eigen::Matrix3d a4_u_nx;
    Eigen::Matrix3d a5_v_ny;
    Eigen::Matrix3d a6_nn;
    Eigen::Matrix3d a7_nx_nx;
    Eigen::Matrix3d a8_ny_ny;
};

LocalIntegrals compute_local_integrals(const mesh::Triangle& triangle);

}  // namespace koshiba::fem
