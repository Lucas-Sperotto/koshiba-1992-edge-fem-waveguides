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

}  // namespace koshiba::algebra
