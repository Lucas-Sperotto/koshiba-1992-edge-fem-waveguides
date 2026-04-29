#pragma once

namespace koshiba::physics {

constexpr double pi = 3.141592653589793238462643383279502884;
constexpr double speed_of_light_m_per_s = 299792458.0;

double k0_from_normalized_frequency(double v,
                                    double scale_t_m,
                                    double n_core,
                                    double n_cladding);

double normalized_frequency_from_k0(double k0,
                                    double scale_t_m,
                                    double n_core,
                                    double n_cladding);

double normalized_propagation_constant(double beta,
                                       double k0,
                                       double n_core,
                                       double n_cladding);

double frequency_ghz_from_k0(double k0);
double k0_from_frequency_ghz(double frequency_ghz);

}  // namespace koshiba::physics
