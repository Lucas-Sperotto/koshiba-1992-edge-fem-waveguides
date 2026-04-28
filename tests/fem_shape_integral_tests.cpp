#include "koshiba/fem/edge_shape.hpp"
#include "koshiba/fem/local_integrals.hpp"

#include <Eigen/Dense>

#include <cassert>
#include <cmath>
#include <array>

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

koshiba::mesh::Triangle general_triangle() {
    return koshiba::mesh::Triangle({{
        {1, 0.0, 0.0},
        {2, 2.0, 1.0},
        {3, 1.0, 3.0},
    }});
}

koshiba::mesh::Triangle scaled_triangle() {
    return koshiba::mesh::Triangle({{
        {1, 0.0, 0.0},
        {2, 3.0, 0.0},
        {3, 0.0, 2.0},
    }});
}

struct QuadraturePoint {
    double l1{};
    double l2{};
    double l3{};
};

constexpr std::array<QuadraturePoint, 3> degree_two_points{{
    {1.0 / 6.0, 1.0 / 6.0, 2.0 / 3.0},
    {1.0 / 6.0, 2.0 / 3.0, 1.0 / 6.0},
    {2.0 / 3.0, 1.0 / 6.0, 1.0 / 6.0},
}};

koshiba::mesh::Point physical_point(const koshiba::mesh::Triangle& triangle,
                                    const QuadraturePoint& point) {
    const auto& vertices = triangle.vertices();
    return {
        point.l1 * vertices[0].x + point.l2 * vertices[1].x + point.l3 * vertices[2].x,
        point.l1 * vertices[0].y + point.l2 * vertices[1].y + point.l3 * vertices[2].y,
    };
}

Eigen::Matrix3d quadrature_a1_uu(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto u = edge_shape.u(xy.y);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * u[static_cast<std::size_t>(row)] *
                                    u[static_cast<std::size_t>(col)];
            }
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_a2_vv(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto v = edge_shape.v(xy.x);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * v[static_cast<std::size_t>(row)] *
                                    v[static_cast<std::size_t>(col)];
            }
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_a3_derivative(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    const auto u_y = edge_shape.u_y();
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            matrix(row, col) = triangle.area() *
                u_y[static_cast<std::size_t>(row)] * u_y[static_cast<std::size_t>(col)];
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_a4_u_nx(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    const auto gradients = triangle.nodal_gradients();
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto u = edge_shape.u(xy.y);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * u[static_cast<std::size_t>(row)] *
                                    gradients[static_cast<std::size_t>(col)].dx;
            }
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_a5_v_ny(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    const auto gradients = triangle.nodal_gradients();
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto v = edge_shape.v(xy.x);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * v[static_cast<std::size_t>(row)] *
                                    gradients[static_cast<std::size_t>(col)].dy;
            }
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_a6_nn(const koshiba::mesh::Triangle& triangle) {
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto n = triangle.shape_values(xy.x, xy.y);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * n[static_cast<std::size_t>(row)] *
                                    n[static_cast<std::size_t>(col)];
            }
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_nodal_gradient_product(const koshiba::mesh::Triangle& triangle,
                                                  bool use_dx) {
    const auto gradients = triangle.nodal_gradients();
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto& row_gradient = gradients[static_cast<std::size_t>(row)];
            const auto& col_gradient = gradients[static_cast<std::size_t>(col)];
            matrix(row, col) = triangle.area() *
                (use_dx ? row_gradient.dx : row_gradient.dy) *
                (use_dx ? col_gradient.dx : col_gradient.dy);
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_auxiliary_v_nx(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    const auto gradients = triangle.nodal_gradients();
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto v = edge_shape.v(xy.x);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * v[static_cast<std::size_t>(row)] *
                                    gradients[static_cast<std::size_t>(col)].dx;
            }
        }
    }
    return matrix;
}

Eigen::Matrix3d quadrature_auxiliary_u_ny(const koshiba::mesh::Triangle& triangle) {
    const koshiba::fem::EdgeShape edge_shape(triangle);
    const auto gradients = triangle.nodal_gradients();
    Eigen::Matrix3d matrix = Eigen::Matrix3d::Zero();
    const double weight = triangle.area() / 3.0;
    for (const auto& point : degree_two_points) {
        const auto xy = physical_point(triangle, point);
        const auto u = edge_shape.u(xy.y);
        for (Eigen::Index row = 0; row < 3; ++row) {
            for (Eigen::Index col = 0; col < 3; ++col) {
                matrix(row, col) += weight * u[static_cast<std::size_t>(row)] *
                                    gradients[static_cast<std::size_t>(col)].dy;
            }
        }
    }
    return matrix;
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

void test_tangential_continuity_at_multiple_points() {
    const koshiba::mesh::Triangle first({{
        {1, 0.0, 0.0},
        {2, 1.0, 0.0},
        {3, 0.0, 1.0},
    }});
    const koshiba::mesh::Triangle second({{
        {2, 1.0, 0.0},
        {4, 1.0, 1.0},
        {3, 0.0, 1.0},
    }});
    const koshiba::fem::EdgeShape first_shape(first);
    const koshiba::fem::EdgeShape second_shape(second);
    const auto& first_coefficients = first_shape.coefficients();
    const auto& second_coefficients = second_shape.coefficients();

    for (double t : {0.25, 0.50, 0.75}) {
        const double x = 1.0 - t;
        const double y = t;
        const auto first_u = first_shape.u(y);
        const auto first_v = first_shape.v(x);
        const auto second_u = second_shape.u(y);
        const auto second_v = second_shape.v(x);

        const double first_tangential =
            first_u[1] * std::cos(first_coefficients.theta[1]) +
            first_v[1] * std::sin(first_coefficients.theta[1]);
        const double second_tangential =
            second_u[2] * std::cos(second_coefficients.theta[2]) +
            second_v[2] * std::sin(second_coefficients.theta[2]);

        expect_close(first_tangential, 1.0);
        expect_close(second_tangential, 1.0);
        expect_close(first_tangential, second_tangential);
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
    const auto triangle = general_triangle();
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

void test_quadrature_matches_a1_to_a8() {
    for (const auto& triangle : {reference_triangle(), general_triangle(), scaled_triangle()}) {
        const auto integrals = koshiba::fem::compute_local_integrals(triangle);

        expect_matrix_close(integrals.a1_uu, quadrature_a1_uu(triangle));
        expect_matrix_close(integrals.a2_vv, quadrature_a2_vv(triangle));
        expect_matrix_close(integrals.a3_derivative, quadrature_a3_derivative(triangle));
        expect_matrix_close(integrals.a4_u_nx, quadrature_a4_u_nx(triangle));
        expect_matrix_close(integrals.a5_v_ny, quadrature_a5_v_ny(triangle));
        expect_matrix_close(integrals.a6_nn, quadrature_a6_nn(triangle));
        expect_matrix_close(integrals.a7_nx_nx, quadrature_nodal_gradient_product(triangle, true));
        expect_matrix_close(integrals.a8_ny_ny, quadrature_nodal_gradient_product(triangle, false));
    }
}

void test_auxiliary_a11_a12_by_quadrature() {
    const auto triangle = general_triangle();
    const koshiba::fem::EdgeShape edge_shape(triangle);
    const auto& edge = edge_shape.coefficients();
    const auto nodal = triangle.nodal_coefficients();
    const auto centroid = triangle.centroid();

    Eigen::Matrix3d expected_a11;
    Eigen::Matrix3d expected_a12;
    for (Eigen::Index row = 0; row < 3; ++row) {
        for (Eigen::Index col = 0; col < 3; ++col) {
            const auto r = static_cast<std::size_t>(row);
            const auto c = static_cast<std::size_t>(col);
            expected_a11(row, col) =
                0.5 * (edge.b[r] - edge.c[r] * centroid.x) * nodal[c].b;
            expected_a12(row, col) =
                0.5 * (edge.a[r] + edge.c[r] * centroid.y) * nodal[c].c;
        }
    }

    expect_matrix_close(expected_a11, quadrature_auxiliary_v_nx(triangle));
    expect_matrix_close(expected_a12, quadrature_auxiliary_u_ny(triangle));
}

}  // namespace

int main() {
    test_edge_shape_coefficients();
    test_tangential_interpolation();
    test_tangential_continuity_at_multiple_points();
    test_reference_integrals();
    test_general_triangle_a7_a8();
    test_quadrature_matches_a1_to_a8();
    test_auxiliary_a11_a12_by_quadrature();
}
