#include "koshiba/algebra/reduced_eigenproblem.hpp"
#include "koshiba/fem/global_assembly.hpp"
#include "koshiba/io/gmsh.hpp"

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <cassert>
#include <cmath>

namespace {

bool is_symmetric(const Eigen::SparseMatrix<double>& matrix) {
    if (matrix.rows() != matrix.cols()) {
        return false;
    }

    const Eigen::SparseMatrix<double> transpose = matrix.transpose();
    const Eigen::SparseMatrix<double> diff = matrix - transpose;
    return diff.norm() < 1.0e-12;
}

void test_global_blocks_from_gmsh_fixture() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);

    assert(blocks.ktt_curl.rows() == 5);
    assert(blocks.ktt_curl.cols() == 5);
    assert(blocks.ktz_u_nx.rows() == 5);
    assert(blocks.ktz_u_nx.cols() == 4);
    assert(blocks.ktz_v_ny.rows() == 5);
    assert(blocks.ktz_v_ny.cols() == 4);
    assert(blocks.mzz_nn.rows() == 4);
    assert(blocks.mzz_nn.cols() == 4);

    assert(is_symmetric(blocks.ktt_curl));
    assert(is_symmetric(blocks.mtt_uu));
    assert(is_symmetric(blocks.mtt_vv));
    assert(is_symmetric(blocks.kzz_nx_nx));
    assert(is_symmetric(blocks.kzz_ny_ny));
    assert(is_symmetric(blocks.mzz_nn));
}

void test_schur_complement_changes_reduced_matrix() {
    Eigen::MatrixXd ktt = Eigen::MatrixXd::Zero(2, 2);
    ktt(0, 0) = 4.0;
    ktt(1, 1) = 5.0;

    Eigen::MatrixXd ktz = Eigen::MatrixXd::Zero(2, 1);
    ktz(0, 0) = 1.0;
    ktz(1, 0) = 2.0;

    Eigen::MatrixXd kzz = Eigen::MatrixXd::Constant(1, 1, 2.0);
    const Eigen::MatrixXd reduced =
        koshiba::algebra::schur_complement_without_inverse(ktt, ktz, kzz, ktz.transpose());

    assert(std::abs(reduced(0, 0) - 3.5) < 1.0e-12);
    assert(std::abs(reduced(0, 1) + 1.0) < 1.0e-12);
    assert(std::abs(reduced(1, 0) + 1.0) < 1.0e-12);
    assert(std::abs(reduced(1, 1) - 3.0) < 1.0e-12);
}

}  // namespace

int main() {
    test_global_blocks_from_gmsh_fixture();
    test_schur_complement_changes_reduced_matrix();
}
