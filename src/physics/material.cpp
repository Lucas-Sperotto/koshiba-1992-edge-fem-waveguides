#include "koshiba/physics/material.hpp"

#include <stdexcept>
#include <string>
#include <utility>

namespace koshiba::physics {

DiagonalCoefficients DiagonalMaterial::coefficients(FieldKind field_kind) const {
    if (eps_rx == 0.0 || eps_ry == 0.0 || eps_rz == 0.0) {
        throw std::invalid_argument("relative permittivity entries must be non-zero");
    }

    if (field_kind == FieldKind::Electric) {
        return {1.0, 1.0, 1.0, eps_rx, eps_ry, eps_rz};
    }

    return {1.0 / eps_rx, 1.0 / eps_ry, 1.0 / eps_rz, 1.0, 1.0, 1.0};
}

MaterialMap::MaterialMap(std::map<int, DiagonalMaterial> materials)
    : materials_(std::move(materials)) {}

MaterialMap MaterialMap::homogeneous(DiagonalMaterial material) {
    return MaterialMap({{0, material}});
}

bool MaterialMap::contains(int physical_tag) const noexcept {
    return materials_.find(physical_tag) != materials_.end() ||
           materials_.find(0) != materials_.end();
}

const DiagonalMaterial& MaterialMap::material_for(int physical_tag) const {
    const auto found = materials_.find(physical_tag);
    if (found != materials_.end()) {
        return found->second;
    }

    const auto fallback = materials_.find(0);
    if (fallback != materials_.end()) {
        return fallback->second;
    }

    throw std::invalid_argument(
        "missing material for physical tag " + std::to_string(physical_tag));
}

DiagonalCoefficients MaterialMap::coefficients_for(int physical_tag,
                                                   FieldKind field_kind) const {
    return material_for(physical_tag).coefficients(field_kind);
}

FieldKind parse_field_kind(const char* value) {
    const std::string text(value == nullptr ? "" : value);
    if (text == "electric" || text == "Electric" || text == "E" || text == "e") {
        return FieldKind::Electric;
    }
    if (text == "magnetic" || text == "Magnetic" || text == "H" || text == "h") {
        return FieldKind::Magnetic;
    }
    throw std::invalid_argument("field_kind must be electric or magnetic");
}

const char* to_string(FieldKind field_kind) noexcept {
    return field_kind == FieldKind::Electric ? "electric" : "magnetic";
}

}  // namespace koshiba::physics
