#include "koshiba/algebra/reduced_eigenproblem.hpp"

#include <cassert>
#include <cmath>

namespace {

void test_schur_complement_without_inverse() {
    Eigen::MatrixXd ktt = Eigen::MatrixXd::Zero(2, 2);
    ktt(0, 0) = 3.0;
    ktt(1, 1) = 4.0;

    Eigen::MatrixXd ktz = Eigen::MatrixXd::Zero(2, 1);
    ktz(0, 0) = 1.0;

    Eigen::MatrixXd kzz = Eigen::MatrixXd::Constant(1, 1, 2.0);
    const Eigen::MatrixXd reduced =
        koshiba::algebra::schur_complement_without_inverse(ktt, ktz, kzz, ktz.transpose());

    assert(std::abs(reduced(0, 0) - 2.5) < 1.0e-12);
    assert(std::abs(reduced(1, 1) - 4.0) < 1.0e-12);
}

void test_dense_reduced_eigenproblem() {
    Eigen::MatrixXd ktt = Eigen::MatrixXd::Zero(2, 2);
    ktt(0, 0) = 2.0;
    ktt(1, 1) = 3.0;

    const Eigen::MatrixXd ktz = Eigen::MatrixXd::Zero(2, 1);
    const Eigen::MatrixXd kzz = Eigen::MatrixXd::Identity(1, 1);
    const Eigen::MatrixXd mass = Eigen::MatrixXd::Identity(2, 2);

    const auto result =
        koshiba::algebra::solve_reduced_generalized_self_adjoint(ktt, ktz, kzz, mass);

    assert(result.eigenvalues.size() == 2);
    assert(std::abs(result.eigenvalues(0) - 2.0) < 1.0e-12);
    assert(std::abs(result.eigenvalues(1) - 3.0) < 1.0e-12);
}

}  // namespace

int main() {
    test_schur_complement_without_inverse();
    test_dense_reduced_eigenproblem();
}
