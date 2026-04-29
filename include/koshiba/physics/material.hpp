#pragma once

#include <map>

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

class MaterialMap {
public:
    MaterialMap() = default;
    explicit MaterialMap(std::map<int, DiagonalMaterial> materials);

    static MaterialMap homogeneous(DiagonalMaterial material);

    bool contains(int physical_tag) const noexcept;
    const DiagonalMaterial& material_for(int physical_tag) const;
    DiagonalCoefficients coefficients_for(int physical_tag, FieldKind field_kind) const;

private:
    std::map<int, DiagonalMaterial> materials_{};
};

FieldKind parse_field_kind(const char* value);
const char* to_string(FieldKind field_kind) noexcept;

}  // namespace koshiba::physics
