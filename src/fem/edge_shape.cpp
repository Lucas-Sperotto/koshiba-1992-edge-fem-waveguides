#include "koshiba/fem/edge_shape.hpp"

#include <Eigen/Dense>

#include <cmath>
#include <stdexcept>

namespace koshiba::fem {
namespace {

// Normalizes an angle to the range [0, pi). The special case atan2(0, -1) = pi
// is mapped to 0 to handle horizontal edges aligned with the negative x-axis
// consistently.
double normalize_angle(double angle) {
    if (angle < 0.0) {
        angle += M_PI;
    }
    // Map pi to 0, ensuring the interval is [0, pi)
    return (angle >= M_PI) ? 0.0 : angle;
}

std::array<mesh::Point, 3> edge_midpoints(const mesh::Triangle& triangle) {
    const auto& vertices = triangle.vertices();
    return {{
        {(vertices[0].x + vertices[1].x) / 2.0, (vertices[0].y + vertices[1].y) / 2.0},
        {(vertices[1].x + vertices[2].x) / 2.0, (vertices[1].y + vertices[2].y) / 2.0},
        {(vertices[2].x + vertices[0].x) / 2.0, (vertices[2].y + vertices[0].y) / 2.0},
    }};
}

}  // namespace

EdgeShape::EdgeShape(const mesh::Triangle& triangle, double tolerance) : triangle_(triangle) {
    const auto& vertices = triangle_.vertices();
    const auto midpoints = edge_midpoints(triangle_);

    Eigen::Matrix3d interpolation;
    for (std::size_t edge = 0; edge < 3; ++edge) {
        const std::size_t k = edge;
        const std::size_t l = (edge + 1) % 3;
        const double dx = vertices[k].x - vertices[l].x;
        const double dy = vertices[k].y - vertices[l].y;
        const double theta = normalize_angle(std::atan2(dy, dx));
        coefficients_.theta[edge] = theta;

        const double cosine = std::cos(theta);
        const double sine = std::sin(theta);
        interpolation(static_cast<Eigen::Index>(edge), 0) = cosine;
        interpolation(static_cast<Eigen::Index>(edge), 1) = sine;
        interpolation(static_cast<Eigen::Index>(edge), 2) =
            midpoints[edge].y * cosine - midpoints[edge].x * sine;
    }

    coefficients_.delta = interpolation.determinant();
    if (std::abs(coefficients_.delta) <= tolerance) {
        throw std::invalid_argument("singular edge interpolation system");
    }

    // Equations (17)-(21) give the closed-form solution of this tangential
    // interpolation system for the three local edge basis functions.
    const Eigen::FullPivLU<Eigen::Matrix3d> solver(interpolation);
    if (!solver.isInvertible()) {
        throw std::invalid_argument("edge interpolation system is not invertible");
    }

    for (std::size_t basis = 0; basis < 3; ++basis) {
        Eigen::Vector3d rhs = Eigen::Vector3d::Zero();
        rhs(static_cast<Eigen::Index>(basis)) = 1.0;
        const Eigen::Vector3d solution = solver.solve(rhs);
        coefficients_.a[basis] = solution(0);
        coefficients_.b[basis] = solution(1);
        coefficients_.c[basis] = solution(2);
    }
}

const EdgeShapeCoefficients& EdgeShape::coefficients() const noexcept {
    return coefficients_;
}

std::array<double, 3> EdgeShape::u(double y) const noexcept {
    return {{
        coefficients_.a[0] + coefficients_.c[0] * y,
        coefficients_.a[1] + coefficients_.c[1] * y,
        coefficients_.a[2] + coefficients_.c[2] * y,
    }};
}

std::array<double, 3> EdgeShape::v(double x) const noexcept {
    return {{
        coefficients_.b[0] - coefficients_.c[0] * x,
        coefficients_.b[1] - coefficients_.c[1] * x,
        coefficients_.b[2] - coefficients_.c[2] * x,
    }};
}

std::array<double, 3> EdgeShape::u_y() const noexcept {
    return coefficients_.c;
}

std::array<double, 3> EdgeShape::v_x() const noexcept {
    return {{
        -coefficients_.c[0],
        -coefficients_.c[1],
        -coefficients_.c[2],
    }};
}

double EdgeShape::tangential_value(std::size_t basis, std::size_t local_edge) const {
    if (basis >= 3 || local_edge >= 3) {
        throw std::out_of_range("edge basis or local edge index out of range");
    }

    const auto midpoints = edge_midpoints(triangle_);
    const auto u_values = u(midpoints[local_edge].y);
    const auto v_values = v(midpoints[local_edge].x);
    const double theta = coefficients_.theta[local_edge];
    return u_values[basis] * std::cos(theta) + v_values[basis] * std::sin(theta);
}

std::array<double, 3> nodal_shape_values(
    const mesh::Triangle& triangle,
    double x,
    double y) noexcept {
    return triangle.shape_values(x, y);
}

std::array<mesh::NodalGradient, 3> nodal_shape_gradients(
    const mesh::Triangle& triangle) noexcept {
    return triangle.nodal_gradients();
}

}  // namespace koshiba::fem
