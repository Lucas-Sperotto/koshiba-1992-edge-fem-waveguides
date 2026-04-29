#include "koshiba/validation/validation_quantities.hpp"

#include "koshiba/physics/normalization.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace koshiba::validation {

double beta_rad_per_m_from_axis_value(double beta_value, const std::string& x_quantity) {
    if (x_quantity == "beta_rad_per_m") {
        return beta_value;
    }
    if (x_quantity == "beta_rad_per_mm") {
        return 1000.0 * beta_value;
    }
    throw std::invalid_argument("unsupported beta axis quantity " + x_quantity);
}

GuidedModeClassification classify_guided_b_from_beta2(double beta2,
                                                       double k0,
                                                       double n_core,
                                                       double n_cladding,
                                                       double guided_tolerance,
                                                       double beta2_tolerance) {
    if (!std::isfinite(beta2)) {
        return {std::numeric_limits<double>::quiet_NaN(), "non_finite"};
    }
    if (beta2 <= beta2_tolerance) {
        return {std::numeric_limits<double>::quiet_NaN(), "non_positive_beta2"};
    }

    const double beta = std::sqrt(beta2);
    const double b = physics::normalized_propagation_constant(
        beta, k0, n_core, n_cladding);
    if (!std::isfinite(b)) {
        return {b, "non_finite"};
    }

    if (b < -guided_tolerance || b > 1.0 + guided_tolerance) {
        return {b, "outside_guided_range"};
    }

    return {b, "propagating"};
}

bool is_valid_validation_status(const std::string& status) noexcept {
    return status == "propagating";
}

}  // namespace koshiba::validation
