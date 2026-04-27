#pragma once

#include <array>

#include "koshiba/mesh/node.hpp"

namespace koshiba::mesh {

struct Point {
    double x{};
    double y{};
};

struct NodalCoefficients {
    double a{};
    double b{};
    double c{};
};

struct NodalGradient {
    double dx{};
    double dy{};
};

class Triangle {
public:
    explicit Triangle(std::array<Node, 3> vertices);

    const std::array<Node, 3>& vertices() const noexcept;

    double twice_signed_area() const noexcept;
    double signed_area() const noexcept;
    double area() const noexcept;
    Point centroid() const noexcept;
    bool is_degenerate(double tolerance) const noexcept;
    bool is_counter_clockwise() const noexcept;

    std::array<NodalCoefficients, 3> nodal_coefficients() const noexcept;
    std::array<NodalGradient, 3> nodal_gradients() const noexcept;
    std::array<double, 3> shape_values(double x, double y) const noexcept;

private:
    std::array<Node, 3> vertices_{};
};

}  // namespace koshiba::mesh
