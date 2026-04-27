#include "koshiba/mesh/triangle.hpp"

#include <cmath>

namespace koshiba::mesh {

namespace {

constexpr std::array<std::array<int, 2>, 3> cyclic_pairs{{
    {{1, 2}},
    {{2, 0}},
    {{0, 1}},
}};

}  // namespace

Triangle::Triangle(std::array<Node, 3> vertices) : vertices_(vertices) {}

const std::array<Node, 3>& Triangle::vertices() const noexcept {
    return vertices_;
}

double Triangle::twice_signed_area() const noexcept {
    const auto& p1 = vertices_[0];
    const auto& p2 = vertices_[1];
    const auto& p3 = vertices_[2];

    // Equação (9): determinante que fornece 2A_e com orientação.
    return (p2.x - p1.x) * (p3.y - p1.y) -
           (p3.x - p1.x) * (p2.y - p1.y);
}

double Triangle::signed_area() const noexcept {
    return 0.5 * twice_signed_area();
}

double Triangle::area() const noexcept {
    return std::abs(signed_area());
}

Point Triangle::centroid() const noexcept {
    return {
        (vertices_[0].x + vertices_[1].x + vertices_[2].x) / 3.0,
        (vertices_[0].y + vertices_[1].y + vertices_[2].y) / 3.0,
    };
}

bool Triangle::is_degenerate(double tolerance) const noexcept {
    return std::abs(twice_signed_area()) <= tolerance;
}

bool Triangle::is_counter_clockwise() const noexcept {
    return twice_signed_area() > 0.0;
}

std::array<NodalCoefficients, 3> Triangle::nodal_coefficients() const noexcept {
    std::array<NodalCoefficients, 3> coefficients{};

    for (std::size_t k = 0; k < coefficients.size(); ++k) {
        const auto [l, m] = cyclic_pairs[k];
        const auto& pl = vertices_[static_cast<std::size_t>(l)];
        const auto& pm = vertices_[static_cast<std::size_t>(m)];

        // Equações (10)-(12): coeficientes do triângulo linear.
        coefficients[k] = {
            pl.x * pm.y - pm.x * pl.y,
            pl.y - pm.y,
            pm.x - pl.x,
        };
    }

    return coefficients;
}

std::array<NodalGradient, 3> Triangle::nodal_gradients() const noexcept {
    std::array<NodalGradient, 3> gradients{};
    const auto coefficients = nodal_coefficients();
    const double denominator = twice_signed_area();

    for (std::size_t k = 0; k < gradients.size(); ++k) {
        gradients[k] = {
            coefficients[k].b / denominator,
            coefficients[k].c / denominator,
        };
    }

    return gradients;
}

std::array<double, 3> Triangle::shape_values(double x, double y) const noexcept {
    std::array<double, 3> values{};
    const auto coefficients = nodal_coefficients();
    const double denominator = twice_signed_area();

    for (std::size_t k = 0; k < values.size(); ++k) {
        values[k] = (coefficients[k].a + coefficients[k].b * x +
                     coefficients[k].c * y) /
                    denominator;
    }

    return values;
}

}  // namespace koshiba::mesh
