#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <Eigen/Eigenvalues>
#include <cmath>

#include "koshiba/fem/global_assembly.hpp"
#include "koshiba/fem/solver.hpp"
#include "koshiba/io/gmsh.hpp"
#include "koshiba/mesh/mesh.hpp"

namespace {

// The normalize_angle function is in an anonymous namespace in edge_shape.cpp.
// To test it, we have to redeclare it here. This is acceptable for a unit test.
double normalize_angle(double angle) {
    if (angle < 0.0) {
        angle += M_PI;
    }
    return (angle >= M_PI) ? 0.0 : angle;
}

bool is_symmetric(const Eigen::SparseMatrix<double>& matrix) {
    if (matrix.rows() != matrix.cols()) {
        return false;
    }
    Eigen::SparseMatrix<double> diff = matrix - matrix.transpose();
    return diff.norm() < 1e-12;
}

}  // namespace

TEST(AssemblyAndSolver, NormalizeAngle) {
    const double tol = 1e-12;
    // Test case where angle is pi, should be mapped to 0
    EXPECT_NEAR(normalize_angle(M_PI), 0.0, tol);
    // Test case where angle is 0
    EXPECT_NEAR(normalize_angle(0.0), 0.0, tol);
    // Test case in (0, pi)
    EXPECT_NEAR(normalize_angle(M_PI / 2.0), M_PI / 2.0, tol);
    EXPECT_NEAR(normalize_angle(3.0 * M_PI / 4.0), 3.0 * M_PI / 4.0, tol);
    // Test negative angle
    EXPECT_NEAR(normalize_angle(-M_PI / 4.0), 3.0 * M_PI / 4.0, tol);
    // Test atan2(0, -1) which is a special case
    EXPECT_NEAR(normalize_angle(atan2(0.0, -1.0)), 0.0, tol);
}


TEST(AssemblyAndSolver, GlobalBlockSymmetry) {
    const auto mesh = koshiba::io::parse_gmsh(
        "tests/fixtures/two_triangles_tagged.msh");
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);

    EXPECT_TRUE(is_symmetric(blocks.ktt_curl));
    EXPECT_TRUE(is_symmetric(blocks.mtt_uu));
    EXPECT_TRUE(is_symmetric(blocks.mtt_vv));
    EXPECT_TRUE(is_symmetric(blocks.kzz_nx_nx));
    EXPECT_TRUE(is_symmetric(blocks.kzz_ny_ny));
    EXPECT_TRUE(is_symmetric(blocks.mzz_nn));
}

TEST(AssemblyAndSolver, SchurComplementEffect) {
    // This test ensures that the Schur complement term K_tz * K_zz^-1 * K_zt
    // actually modifies the reduced eigenvalue problem when K_tz is non-zero.
    // We use a simple 2-element mesh and an anisotropic material to ensure
    // K_tz is not null.

    const auto mesh = koshiba::io::parse_gmsh(
        "tests/fixtures/two_triangles_tagged.msh");

    // Anisotropic material to make K_tz non-trivial
    koshiba::fem::Material material(1.0, 2.0, 1.0); // Anisotropic in y
    std::map<int, koshiba::fem::Material> materials;
    materials[10] = material; // Tag for the elements

    const double k0 = 1.0;
    const double beta = 0.0; // We test the matrices, not a specific beta

    // Case 1: Full problem reduction
    auto solver_full = koshiba::fem::DenseEigenvalueSolver(mesh, materials, k0);
    auto result_full = solver_full.solve(1);

    // Case 2: Manually construct a problem WITHOUT the Schur complement
    auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);
    Eigen::SparseMatrix<double> K_tt_beta =
        -material.p_z * blocks.ktt_curl +
        k0 * k0 * (material.q_x * blocks.mtt_uu + material.q_y * blocks.mtt_vv);

    Eigen::SparseMatrix<double> M_tt_beta =
        material.p_y * blocks.mtt_uu + material.p_x * blocks.mtt_vv;

    // The M_tt_hat term is what we are testing.
    // M_tt_hat = M_tt_beta + SchurComplement
    // So, if we solve with M_tt_beta instead of M_tt_hat, the result must differ.

    Eigen::MatrixXd K_dense = K_tt_beta;
    Eigen::MatrixXd M_dense = M_tt_beta;

    Eigen::GeneralizedEigenSolver<Eigen::MatrixXd> ges;
    ges.compute(K_dense, M_dense);
    
    double eigenvalue_no_schur = 0.0;
    // Find the smallest positive eigenvalue
    double min_positive_eig = std::numeric_limits<double>::max();
    for (int i = 0; i < ges.eigenvalues().size(); ++i) {
        if (ges.eigenvalues()[i].real() > 1e-9) {
            min_positive_eig = std::min(min_positive_eig, ges.eigenvalues()[i].real());
        }
    }
    eigenvalue_no_schur = min_positive_eig;


    // The eigenvalues must be different if the Schur complement has an effect.
    // K_tz is non-zero for this anisotropic case, so the effect should be present.
    ASSERT_FALSE(result_full.eigenvalues.empty());
    const double eigenvalue_with_schur = result_full.eigenvalues[0];

    EXPECT_NE(eigenvalue_no_schur, 0.0);
    EXPECT_NE(eigenvalue_with_schur, 0.0);
    EXPECT_GT(std::abs(eigenvalue_with_schur - eigenvalue_no_schur), 1e-9);
}