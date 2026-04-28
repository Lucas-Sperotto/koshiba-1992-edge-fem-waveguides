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

void test_effective_mass_without_inverse() {
    Eigen::MatrixXd mass = Eigen::MatrixXd::Identity(2, 2);

    Eigen::MatrixXd ktz = Eigen::MatrixXd::Zero(2, 1);
    ktz(0, 0) = 1.0;
    ktz(1, 0) = 2.0;

    Eigen::MatrixXd kzz = Eigen::MatrixXd::Constant(1, 1, 2.0);
    const Eigen::MatrixXd effective_mass =
        koshiba::algebra::effective_mass_without_inverse(mass, ktz, kzz, ktz.transpose());

    assert(std::abs(effective_mass(0, 0) - 1.5) < 1.0e-12);
    assert(std::abs(effective_mass(0, 1) - 1.0) < 1.0e-12);
    assert(std::abs(effective_mass(1, 0) - 1.0) < 1.0e-12);
    assert(std::abs(effective_mass(1, 1) - 3.0) < 1.0e-12);
}

void test_beta_squared_eigenproblem_uses_effective_mass() {
    Eigen::MatrixXd ktt = Eigen::MatrixXd::Identity(1, 1) * 6.0;
    Eigen::MatrixXd ktz = Eigen::MatrixXd::Identity(1, 1) * 2.0;
    Eigen::MatrixXd kzz = Eigen::MatrixXd::Identity(1, 1) * 4.0;
    Eigen::MatrixXd mass = Eigen::MatrixXd::Identity(1, 1) * 2.0;

    const auto result =
        koshiba::algebra::solve_beta_squared_self_adjoint(ktt, ktz, kzz, mass);

    assert(result.beta2.size() == 1);
    assert(std::abs(result.effective_mass(0, 0) - 3.0) < 1.0e-12);
    assert(std::abs(result.beta2(0) - 2.0) < 1.0e-12);
}

void test_recovered_axial_field_satisfies_partitioned_system() {
    Eigen::MatrixXd ktt = Eigen::MatrixXd::Zero(2, 2);
    ktt(0, 0) = 8.0;
    ktt(1, 1) = 11.0;

    Eigen::MatrixXd ktz = Eigen::MatrixXd::Zero(2, 1);
    ktz(0, 0) = 1.0;
    ktz(1, 0) = -0.5;

    Eigen::MatrixXd kzz = Eigen::MatrixXd::Identity(1, 1) * 3.0;
    Eigen::MatrixXd mass = Eigen::MatrixXd::Identity(2, 2) * 2.0;

    const auto result =
        koshiba::algebra::solve_beta_squared_self_adjoint(ktt, ktz, kzz, mass);

    for (Eigen::Index mode = 0; mode < result.beta2.size(); ++mode) {
        const double beta2 = result.beta2(mode);
        assert(beta2 > 0.0);
        const double beta = std::sqrt(beta2);
        const Eigen::VectorXd phi_t = result.eigenvectors.col(mode);
        const Eigen::VectorXd phi_z =
            koshiba::algebra::recover_axial_field_without_inverse(beta, ktz, kzz, phi_t);

        const Eigen::VectorXd first_residual =
            ktt * phi_t - beta * ktz * phi_z - beta2 * mass * phi_t;
        const Eigen::VectorXd second_residual =
            -beta * ktz.transpose() * phi_t + kzz * phi_z;

        assert(first_residual.norm() < 1.0e-10);
        assert(second_residual.norm() < 1.0e-10);
    }
}

}  // namespace

int main() {
    test_schur_complement_without_inverse();
    test_dense_reduced_eigenproblem();
    test_effective_mass_without_inverse();
    test_beta_squared_eigenproblem_uses_effective_mass();
    test_recovered_axial_field_satisfies_partitioned_system();
}
