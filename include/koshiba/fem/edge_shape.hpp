#pragma once

#include <array>
#include <cstddef>

#include "koshiba/mesh/triangle.hpp"

namespace koshiba::fem {

struct EdgeShapeCoefficients {
    std::array<double, 3> a{};
    std::array<double, 3> b{};
    std::array<double, 3> c{};
    std::array<double, 3> theta{};
    double delta{};
};

class EdgeShape {
public:
    explicit EdgeShape(const mesh::Triangle& triangle, double tolerance = 1.0e-12);

    const EdgeShapeCoefficients& coefficients() const noexcept;

    std::array<double, 3> u(double y) const noexcept;
    std::array<double, 3> v(double x) const noexcept;
    std::array<double, 3> u_y() const noexcept;
    std::array<double, 3> v_x() const noexcept;

    double tangential_value(std::size_t basis, std::size_t local_edge) const;

private:
    mesh::Triangle triangle_;
    EdgeShapeCoefficients coefficients_{};
};

std::array<double, 3> nodal_shape_values(
    const mesh::Triangle& triangle,
    double x,
    double y) noexcept;

std::array<mesh::NodalGradient, 3> nodal_shape_gradients(
    const mesh::Triangle& triangle) noexcept;

}  // namespace koshiba::fem
