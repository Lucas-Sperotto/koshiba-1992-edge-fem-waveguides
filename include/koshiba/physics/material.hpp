#pragma once

namespace koshiba::physics {

enum class FieldKind {
    Electric,
    Magnetic,
};

struct DiagonalCoefficients {
    double p_x{};
    double p_y{};
    double p_z{};
    double q_x{};
    double q_y{};
    double q_z{};
};

struct DiagonalMaterial {
    double eps_rx{1.0};
    double eps_ry{1.0};
    double eps_rz{1.0};

    DiagonalCoefficients coefficients(FieldKind field_kind) const;
};

FieldKind parse_field_kind(const char* value);
const char* to_string(FieldKind field_kind) noexcept;

}  // namespace koshiba::physics
