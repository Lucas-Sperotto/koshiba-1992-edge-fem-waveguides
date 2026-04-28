#include "koshiba/physics/material.hpp"

#include <stdexcept>
#include <string>

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
