#pragma once

#include <string>

namespace koshiba::validation {

struct GuidedModeClassification {
    double b{};
    std::string status{};
};

double beta_rad_per_m_from_axis_value(double beta_value, const std::string& x_quantity);

GuidedModeClassification classify_guided_b_from_beta2(double beta2,
                                                       double k0,
                                                       double n_core,
                                                       double n_cladding,
                                                       double guided_tolerance = 1.0e-10,
                                                       double beta2_tolerance = 1.0e-10);

bool is_valid_validation_status(const std::string& status) noexcept;

}  // namespace koshiba::validation
