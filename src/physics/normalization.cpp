#include "koshiba/physics/normalization.hpp"

#include <cmath>
#include <stdexcept>

namespace koshiba::physics {

namespace {

double index_contrast(double n_core, double n_cladding) {
    const double contrast = n_core * n_core - n_cladding * n_cladding;
    if (contrast <= 0.0) {
        throw std::invalid_argument("n_core must be greater than n_cladding");
    }
    return std::sqrt(contrast);
}

}  // namespace

double k0_from_normalized_frequency(double v,
                                    double scale_t_m,
                                    double n_core,
                                    double n_cladding) {
    if (scale_t_m <= 0.0) {
        throw std::invalid_argument("scale_t_m must be positive");
    }
    return pi * v / (scale_t_m * index_contrast(n_core, n_cladding));
}

double normalized_frequency_from_k0(double k0,
                                    double scale_t_m,
                                    double n_core,
                                    double n_cladding) {
    if (k0 <= 0.0) {
        throw std::invalid_argument("k0 must be positive");
    }
    if (scale_t_m <= 0.0) {
        throw std::invalid_argument("scale_t_m must be positive");
    }
    return k0 * scale_t_m * index_contrast(n_core, n_cladding) / pi;
}

double normalized_propagation_constant(double beta,
                                       double k0,
                                       double n_core,
                                       double n_cladding) {
    if (k0 <= 0.0) {
        throw std::invalid_argument("k0 must be positive");
    }
    const double denominator = n_core * n_core - n_cladding * n_cladding;
    if (denominator <= 0.0) {
        throw std::invalid_argument("n_core must be greater than n_cladding");
    }
    return ((beta / k0) * (beta / k0) - n_cladding * n_cladding) / denominator;
}

double frequency_ghz_from_k0(double k0) {
    if (k0 <= 0.0) {
        throw std::invalid_argument("k0 must be positive");
    }
    return k0 * speed_of_light_m_per_s / (2.0 * pi) / 1.0e9;
}

double k0_from_frequency_ghz(double frequency_ghz) {
    if (frequency_ghz <= 0.0) {
        throw std::invalid_argument("frequency_ghz must be positive");
    }
    return 2.0 * pi * frequency_ghz * 1.0e9 / speed_of_light_m_per_s;
}

}  // namespace koshiba::physics
