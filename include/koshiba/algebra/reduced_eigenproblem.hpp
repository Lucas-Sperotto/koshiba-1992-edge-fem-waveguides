#pragma once

#include <Eigen/Dense>

namespace koshiba::algebra {

struct ReducedEigenResult {
    Eigen::VectorXd eigenvalues;
    Eigen::MatrixXd eigenvectors;
    Eigen::MatrixXd reduced_stiffness;
};

Eigen::MatrixXd schur_complement_without_inverse(
    const Eigen::MatrixXd& ktt,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& kzt);

ReducedEigenResult solve_reduced_generalized_self_adjoint(
    const Eigen::MatrixXd& ktt,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& mass);

}  // namespace koshiba::algebra
