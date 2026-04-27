#include "koshiba/fem/edge_shape.hpp"
#include "koshiba/fem/local_integrals.hpp"

#include <Eigen/Dense>

#include <cassert>
#include <cmath>

namespace {

constexpr double tolerance = 1.0e-12;

bool close(double lhs, double rhs) {
    return std::abs(lhs - rhs) <= tolerance;
}

void expect_close(double lhs, double rhs) {
    assert(close(lhs, rhs));
}

void expect_matrix_close(const Eigen::Matrix3d& actual, const Eigen::Matrix3d& expected) {
    for (Eigen::Index row = 0; row < actual.rows(); ++row) {
        for (Eigen::Index col = 0; col < actual.cols(); ++col) {
            expect_close(actual(row, col), expected(row, col));
        }
    }
}

koshiba::mesh::Triangle reference_triangle() {
    return koshiba::mesh::Triangle({{
        {1, 0.0, 0.0},
        {2, 1.0, 0.0},
        {3, 0.0, 1.0},
    }});
}

void test_edge_shape_coefficients() {
    const koshiba::fem::EdgeShape edge_shape(reference_triangle());
    const auto& coefficients = edge_shape.coefficients();
    const double sqrt2 = std::sqrt(2.0);

    expect_close(coefficients.theta[0], 0.0);
    expect_close(coefficients.theta[1], 3.0 * std::acos(-1.0) / 4.0);
    expect_close(coefficients.theta[2], std::acos(-1.0) / 2.0);
    expect_close(coefficients.delta, sqrt2 / 2.0);

    expect_close(coefficients.a[0], 1.0);
    expect_close(coefficients.a[1], 0.0);
    expect_close(coefficients.a[2], 0.0);

    expect_close(coefficients.b[0], 0.0);
    expect_close(coefficients.b[1], 0.0);
    expect_close(coefficients.b[2], 1.0);

    expect_close(coefficients.c[0], -1.0);
    expect_close(coefficients.c[1], -sqrt2);
    expect_close(coefficients.c[2], 1.0);

    const auto u_y = edge_shape.u_y();
    const auto v_x = edge_shape.v_x();
    expect_close(u_y[0], -1.0);
    expect_close(u_y[1], -sqrt2);
    expect_close(u_y[2], 1.0);
    expect_close(v_x[0], 1.0);
    expect_close(v_x[1], sqrt2);
    expect_close(v_x[2], -1.0);
}

void test_tangential_interpolation() {
    const koshiba::fem::EdgeShape edge_shape(reference_triangle());

    for (std::size_t basis = 0; basis < 3; ++basis) {
        for (std::size_t edge = 0; edge < 3; ++edge) {
            expect_close(edge_shape.tangential_value(basis, edge), basis == edge ? 1.0 : 0.0);
        }
    }
}

void test_reference_integrals() {
    const auto integrals = koshiba::fem::compute_local_integrals(reference_triangle());
    const double sqrt2 = std::sqrt(2.0);

    Eigen::Matrix3d expected_a1;
    expected_a1 << 1.0 / 4.0, -sqrt2 / 12.0, 1.0 / 12.0,
        -sqrt2 / 12.0, 1.0 / 6.0, -sqrt2 / 12.0,
        1.0 / 12.0, -sqrt2 / 12.0, 1.0 / 12.0;
    expect_matrix_close(integrals.a1_uu, expected_a1);

    Eigen::Matrix3d expected_a2;
    expected_a2 << 1.0 / 12.0, sqrt2 / 12.0, 1.0 / 12.0,
        sqrt2 / 12.0, 1.0 / 6.0, sqrt2 / 12.0,
        1.0 / 12.0, sqrt2 / 12.0, 1.0 / 4.0;
    expect_matrix_close(integrals.a2_vv, expected_a2);

    Eigen::Matrix3d expected_a3;
    expected_a3 << 0.5, sqrt2 / 2.0, -0.5,
        sqrt2 / 2.0, 1.0, -sqrt2 / 2.0,
        -0.5, -sqrt2 / 2.0, 0.5;
    expect_matrix_close(integrals.a3_derivative, expected_a3);

    Eigen::Matrix3d expected_a4;
    expected_a4 << -1.0 / 3.0, 1.0 / 3.0, 0.0,
        sqrt2 / 6.0, -sqrt2 / 6.0, 0.0,
        -1.0 / 6.0, 1.0 / 6.0, 0.0;
    expect_matrix_close(integrals.a4_u_nx, expected_a4);

    Eigen::Matrix3d expected_a5;
    expected_a5 << -1.0 / 6.0, 0.0, 1.0 / 6.0,
        -sqrt2 / 6.0, 0.0, sqrt2 / 6.0,
        -1.0 / 3.0, 0.0, 1.0 / 3.0;
    expect_matrix_close(integrals.a5_v_ny, expected_a5);

    Eigen::Matrix3d expected_a6;
    expected_a6 << 2.0, 1.0, 1.0,
        1.0, 2.0, 1.0,
        1.0, 1.0, 2.0;
    expected_a6 *= 1.0 / 24.0;
    expect_matrix_close(integrals.a6_nn, expected_a6);

    Eigen::Matrix3d expected_a7;
    expected_a7 << 1.0, -1.0, 0.0,
        -1.0, 1.0, 0.0,
        0.0, 0.0, 0.0;
    expected_a7 *= 0.5;
    expect_matrix_close(integrals.a7_nx_nx, expected_a7);

    Eigen::Matrix3d expected_a8;
    expected_a8 << 1.0, 0.0, -1.0,
        0.0, 0.0, 0.0,
        -1.0, 0.0, 1.0;
    expected_a8 *= 0.5;
    expect_matrix_close(integrals.a8_ny_ny, expected_a8);
}

void test_general_triangle_a7_a8() {
    const koshiba::mesh::Triangle triangle({{
        {1, 0.0, 0.0},
        {2, 2.0, 1.0},
        {3, 1.0, 3.0},
    }});
    const auto integrals = koshiba::fem::compute_local_integrals(triangle);

    Eigen::Matrix3d expected_a7;
    expected_a7 << 4.0, -6.0, 2.0,
        -6.0, 9.0, -3.0,
        2.0, -3.0, 1.0;
    expected_a7 *= 0.1;
    expect_matrix_close(integrals.a7_nx_nx, expected_a7);

    Eigen::Matrix3d expected_a8;
    expected_a8 << 1.0, 1.0, -2.0,
        1.0, 1.0, -2.0,
        -2.0, -2.0, 4.0;
    expected_a8 *= 0.1;
    expect_matrix_close(integrals.a8_ny_ny, expected_a8);
}

}  // namespace

int main() {
    test_edge_shape_coefficients();
    test_tangential_interpolation();
    test_reference_integrals();
    test_general_triangle_a7_a8();
}
