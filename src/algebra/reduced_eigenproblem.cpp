#include "koshiba/algebra/reduced_eigenproblem.hpp"

#include <stdexcept>

namespace koshiba::algebra {

Eigen::MatrixXd schur_complement_without_inverse(
    const Eigen::MatrixXd& ktt,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& kzt) {
    if (kzz.rows() != kzz.cols()) {
        throw std::invalid_argument("Kzz must be square");
    }
    if (ktz.cols() != kzz.rows() || kzt.rows() != kzz.cols()) {
        throw std::invalid_argument("incompatible Ktz, Kzz, and Kzt dimensions");
    }
    if (ktt.rows() != ktz.rows() || ktt.cols() != kzt.cols()) {
        throw std::invalid_argument("incompatible Ktt dimensions");
    }

    const Eigen::FullPivLU<Eigen::MatrixXd> solver(kzz);
    if (!solver.isInvertible()) {
        throw std::runtime_error("Kzz is singular in reduced eigenproblem");
    }

    return ktt - ktz * solver.solve(kzt);
}

ReducedEigenResult solve_reduced_generalized_self_adjoint(
    const Eigen::MatrixXd& ktt,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& mass) {
    const Eigen::MatrixXd reduced =
        schur_complement_without_inverse(ktt, ktz, kzz, ktz.transpose());
    if (mass.rows() != reduced.rows() || mass.cols() != reduced.cols()) {
        throw std::invalid_argument("mass matrix dimensions do not match reduced stiffness");
    }

    Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(reduced, mass);
    if (eigensolver.info() != Eigen::Success) {
        throw std::runtime_error("dense generalized eigenproblem failed");
    }

    return {eigensolver.eigenvalues(), eigensolver.eigenvectors(), reduced};
}

Eigen::MatrixXd effective_mass_without_inverse(
    const Eigen::MatrixXd& mass,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& kzt) {
    if (kzz.rows() != kzz.cols()) {
        throw std::invalid_argument("Kzz must be square");
    }
    if (ktz.cols() != kzz.rows() || kzt.rows() != kzz.cols()) {
        throw std::invalid_argument("incompatible Ktz, Kzz, and Kzt dimensions");
    }
    if (mass.rows() != ktz.rows() || mass.cols() != kzt.cols()) {
        throw std::invalid_argument("incompatible mass dimensions");
    }

    Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXd> solver(kzz);
    if (solver.rank() == 0) {
        throw std::runtime_error("Kzz has zero rank in beta squared eigenproblem");
    }

    return mass + ktz * solver.solve(kzt);
}

Eigen::VectorXd recover_axial_field_without_inverse(
    double beta,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::VectorXd& transverse_field) {
    if (kzz.rows() != kzz.cols()) {
        throw std::invalid_argument("Kzz must be square");
    }
    if (ktz.cols() != kzz.rows()) {
        throw std::invalid_argument("incompatible Ktz and Kzz dimensions");
    }
    if (ktz.rows() != transverse_field.size()) {
        throw std::invalid_argument("transverse field dimensions do not match Ktz");
    }

    Eigen::CompleteOrthogonalDecomposition<Eigen::MatrixXd> solver(kzz);
    if (solver.rank() == 0) {
        throw std::runtime_error("Kzz has zero rank in axial field recovery");
    }

    return beta * solver.solve(ktz.transpose() * transverse_field);
}

BetaSquaredEigenResult solve_beta_squared_self_adjoint(
    const Eigen::MatrixXd& ktt,
    const Eigen::MatrixXd& ktz,
    const Eigen::MatrixXd& kzz,
    const Eigen::MatrixXd& mass) {
    if (ktt.rows() != ktt.cols()) {
        throw std::invalid_argument("Ktt must be square");
    }

    const Eigen::MatrixXd effective_mass =
        effective_mass_without_inverse(mass, ktz, kzz, ktz.transpose());
    if (effective_mass.rows() != ktt.rows() || effective_mass.cols() != ktt.cols()) {
        throw std::invalid_argument("effective mass dimensions do not match Ktt");
    }

    Eigen::GeneralizedSelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(ktt, effective_mass);
    if (eigensolver.info() != Eigen::Success) {
        throw std::runtime_error("beta squared generalized eigenproblem failed");
    }

    return {eigensolver.eigenvalues(), eigensolver.eigenvectors(), effective_mass};
}

}  // namespace koshiba::algebra
