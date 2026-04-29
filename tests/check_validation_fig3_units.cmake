set(FIG3_CSV "${ROOT}/data/output/validation/fig3a_microstrip_isotropic.csv")

if(NOT EXISTS "${FIG3_CSV}")
    message(FATAL_ERROR "missing Figure 3 CSV: ${FIG3_CSV}")
endif()

file(READ "${FIG3_CSV}" CONTENT)
if(NOT CONTENT MATCHES "beta_rad_per_mm,frequency_ghz")
    message(FATAL_ERROR "Figure 3 CSV does not use beta_rad_per_mm/frequency_ghz")
endif()

if(NOT CONTENT MATCHES "fig3a_microstrip_isotropic,mode_0,0\\.2,")
    message(FATAL_ERROR "Figure 3 CSV should preserve beta axis values in rad/mm")
endif()
