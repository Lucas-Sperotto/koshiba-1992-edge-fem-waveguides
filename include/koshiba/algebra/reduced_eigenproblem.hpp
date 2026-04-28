#pragma once

#include <Eigen/Dense>

namespace koshiba::algebra {

struct ReducedEigenResult {
    Eigen::VectorXd eigenvalues;
    Eigen::MatrixXd eigenvectors;
    Eigen::MatrixXd reduced_stiffness;
};

struct BetaSquaredEigenResult {
    Eigen::VectorXd beta2;
    Eigen::MatrixXd eigenvectors;
    Eigen::MatrixXd effective_mass;
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

Eigen::MatrixXd effective_mass_without_inverse(
    const Eigen::MatrixXd& mass,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& kzt);

Eigen::VectorXd recover_axial_field_without_inverse(
    double beta,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::VectorXd& transverse_field);

BetaSquaredEigenResult solve_beta_squared_self_adjoint(
    const Eigen::MatrixXd& ktt,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& mass);

}  // namespace koshiba::algebra
