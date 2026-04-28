#include "koshiba/fem/global_assembly.hpp"

#include <cassert>
#include <cmath>

namespace {

koshiba::mesh::Mesh two_triangle_mesh() {
    return koshiba::mesh::Mesh(
        {
            {1, 0.0, 0.0},
            {2, 1.0, 0.0},
            {3, 0.0, 1.0},
            {4, 1.0, 1.0},
        },
        {
            {1, {{1, 2, 3}}, 21},
            {2, {{2, 4, 3}}, 21},
        });
}

double sparse_sum(const Eigen::SparseMatrix<double>& matrix) {
    double sum = 0.0;
    for (int outer = 0; outer < matrix.outerSize(); ++outer) {
        for (Eigen::SparseMatrix<double>::InnerIterator it(matrix, outer); it; ++it) {
            sum += it.value();
        }
    }
    return sum;
}

bool is_symmetric(const Eigen::SparseMatrix<double>& matrix) {
    if (matrix.rows() != matrix.cols()) {
        return false;
    }
    const Eigen::SparseMatrix<double> transpose = matrix.transpose();
    const Eigen::SparseMatrix<double> diff = matrix - transpose;
    return diff.norm() < 1.0e-12;
}

void test_global_block_dimensions() {
    const auto mesh = two_triangle_mesh();
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);

    assert(blocks.ktt_curl.rows() == 5);
    assert(blocks.ktt_curl.cols() == 5);
    assert(blocks.ktz_u_nx.rows() == 5);
    assert(blocks.ktz_u_nx.cols() == 4);
    assert(blocks.kzz_nx_nx.rows() == 4);
    assert(blocks.kzz_nx_nx.cols() == 4);
    assert(blocks.mzz_nn.rows() == 4);
    assert(blocks.mzz_nn.cols() == 4);
}

void test_global_mass_sum() {
    const auto mesh = two_triangle_mesh();
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);
    assert(std::abs(sparse_sum(blocks.mzz_nn) - 1.0) < 1.0e-12);
}

void test_shared_edge_receives_two_contributions() {
    const auto mesh = two_triangle_mesh();
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);
    const auto shared_edge = mesh.element_edges()[0].edge_ids[1];
    assert(blocks.ktt_curl.coeff(static_cast<Eigen::Index>(shared_edge),
                                 static_cast<Eigen::Index>(shared_edge)) > 0.0);
}

void test_expected_global_blocks_are_symmetric() {
    const auto mesh = two_triangle_mesh();
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);

    assert(is_symmetric(blocks.ktt_curl));
    assert(is_symmetric(blocks.mtt_uu));
    assert(is_symmetric(blocks.mtt_vv));
    assert(is_symmetric(blocks.kzz_nx_nx));
    assert(is_symmetric(blocks.kzz_ny_ny));
    assert(is_symmetric(blocks.mzz_nn));
}

}  // namespace

int main() {
    test_global_block_dimensions();
    test_global_mass_sum();
    test_shared_edge_receives_two_contributions();
    test_expected_global_blocks_are_symmetric();
}
