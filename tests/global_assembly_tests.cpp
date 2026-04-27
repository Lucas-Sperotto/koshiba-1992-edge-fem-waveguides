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

}  // namespace

int main() {
    test_global_block_dimensions();
    test_global_mass_sum();
    test_shared_edge_receives_two_contributions();
}
