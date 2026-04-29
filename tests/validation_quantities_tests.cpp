#include "koshiba/validation/validation_quantities.hpp"

#include <cassert>
#include <cmath>
#include <stdexcept>

namespace {

void test_beta_rad_per_mm_converts_to_rad_per_m() {
    const double converted =
        koshiba::validation::beta_rad_per_m_from_axis_value(1.25, "beta_rad_per_mm");
    assert(std::abs(converted - 1250.0) < 1.0e-12);
}

void test_beta_rad_per_m_is_preserved() {
    const double converted =
        koshiba::validation::beta_rad_per_m_from_axis_value(1.25, "beta_rad_per_m");
    assert(std::abs(converted - 1.25) < 1.0e-12);
}

void test_unknown_beta_unit_throws() {
    bool threw = false;
    try {
        (void)koshiba::validation::beta_rad_per_m_from_axis_value(1.0, "beta_cycles_per_m");
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

void test_classifies_negative_b_outside_guided_range() {
    const auto classification =
        koshiba::validation::classify_guided_b_from_beta2(0.25, 1.0, 1.5, 1.0);
    assert(classification.status == "outside_guided_range");
    assert(classification.b < 0.0);
}

void test_classifies_b_inside_guided_range_as_propagating() {
    const double n_core = 1.5;
    const double n_cladding = 1.0;
    const double target_b = 0.5;
    const double beta_over_k0 =
        std::sqrt(n_cladding * n_cladding +
                  target_b * (n_core * n_core - n_cladding * n_cladding));

    const auto classification =
        koshiba::validation::classify_guided_b_from_beta2(
            beta_over_k0 * beta_over_k0, 1.0, n_core, n_cladding);
    assert(classification.status == "propagating");
    assert(std::abs(classification.b - target_b) < 1.0e-12);
}

void test_classifies_non_positive_beta2() {
    const auto classification =
        koshiba::validation::classify_guided_b_from_beta2(0.0, 1.0, 1.5, 1.0);
    assert(classification.status == "non_positive_beta2");
    assert(std::isnan(classification.b));
}

}  // namespace

int main() {
    test_beta_rad_per_mm_converts_to_rad_per_m();
    test_beta_rad_per_m_is_preserved();
    test_unknown_beta_unit_throws();
    test_classifies_negative_b_outside_guided_range();
    test_classifies_b_inside_guided_range_as_propagating();
    test_classifies_non_positive_beta2();
}
