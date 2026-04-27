#include "koshiba/mesh/triangle.hpp"

#include <array>
#include <cassert>
#include <cmath>

namespace {

constexpr double tolerance = 1.0e-14;

bool close(double lhs, double rhs) {
    return std::abs(lhs - rhs) <= tolerance;
}

void expect_close(double lhs, double rhs) {
    assert(close(lhs, rhs));
}

koshiba::mesh::Triangle reference_triangle() {
    return koshiba::mesh::Triangle({{
        {0, 0.0, 0.0},
        {1, 1.0, 0.0},
        {2, 0.0, 1.0},
    }});
}

void test_reference_area() {
    const auto triangle = reference_triangle();

    expect_close(triangle.twice_signed_area(), 1.0);
    expect_close(triangle.signed_area(), 0.5);
    expect_close(triangle.area(), 0.5);
    assert(triangle.is_counter_clockwise());
    assert(!triangle.is_degenerate(tolerance));
}

void test_negative_orientation() {
    const koshiba::mesh::Triangle triangle({{
        {0, 0.0, 0.0},
        {1, 0.0, 1.0},
        {2, 1.0, 0.0},
    }});

    expect_close(triangle.twice_signed_area(), -1.0);
    expect_close(triangle.signed_area(), -0.5);
    expect_close(triangle.area(), 0.5);
    assert(!triangle.is_counter_clockwise());
}

void test_centroid() {
    const auto triangle = reference_triangle();
    const auto centroid = triangle.centroid();

    expect_close(centroid.x, 1.0 / 3.0);
    expect_close(centroid.y, 1.0 / 3.0);
}

void test_nodal_coefficients() {
    const auto triangle = reference_triangle();
    const auto coefficients = triangle.nodal_coefficients();

    expect_close(coefficients[0].a, 1.0);
    expect_close(coefficients[0].b, -1.0);
    expect_close(coefficients[0].c, -1.0);

    expect_close(coefficients[1].a, 0.0);
    expect_close(coefficients[1].b, 1.0);
    expect_close(coefficients[1].c, 0.0);

    expect_close(coefficients[2].a, 0.0);
    expect_close(coefficients[2].b, 0.0);
    expect_close(coefficients[2].c, 1.0);
}

void test_shape_values_at_vertices() {
    const auto triangle = reference_triangle();
    const std::array<koshiba::mesh::Point, 3> points{{
        {0.0, 0.0},
        {1.0, 0.0},
        {0.0, 1.0},
    }};

    for (std::size_t vertex = 0; vertex < points.size(); ++vertex) {
        const auto values = triangle.shape_values(points[vertex].x, points[vertex].y);

        for (std::size_t k = 0; k < values.size(); ++k) {
            expect_close(values[k], vertex == k ? 1.0 : 0.0);
        }
    }
}

void test_partition_of_unity() {
    const auto triangle = reference_triangle();
    const auto values = triangle.shape_values(0.25, 0.25);

    expect_close(values[0] + values[1] + values[2], 1.0);
}

void test_nodal_gradients() {
    const auto triangle = reference_triangle();
    const auto gradients = triangle.nodal_gradients();

    expect_close(gradients[0].dx, -1.0);
    expect_close(gradients[0].dy, -1.0);

    expect_close(gradients[1].dx, 1.0);
    expect_close(gradients[1].dy, 0.0);

    expect_close(gradients[2].dx, 0.0);
    expect_close(gradients[2].dy, 1.0);
}

void test_degenerate_triangle_detection() {
    const koshiba::mesh::Triangle triangle({{
        {0, 0.0, 0.0},
        {1, 1.0, 1.0},
        {2, 2.0, 2.0},
    }});

    expect_close(triangle.twice_signed_area(), 0.0);
    assert(triangle.is_degenerate(tolerance));
}

}  // namespace

int main() {
    test_reference_area();
    test_negative_orientation();
    test_centroid();
    test_nodal_coefficients();
    test_shape_values_at_vertices();
    test_partition_of_unity();
    test_nodal_gradients();
    test_degenerate_triangle_detection();
}
