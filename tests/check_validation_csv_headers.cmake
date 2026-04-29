set(EXPECTED "case_id,curve_id,x,y,x_quantity,y_quantity,mode_label,field_kind,status")

set(CSV_FILES
    "${ROOT}/data/output/validation/fig3a_microstrip_isotropic.csv"
    "${ROOT}/data/output/validation/fig3b_microstrip_anisotropic.csv"
    "${ROOT}/data/output/validation/fig5a_rect_ex_low_contrast.csv"
    "${ROOT}/data/output/validation/fig5b_rect_ey_low_contrast.csv"
    "${ROOT}/data/output/validation/fig5c_rect_ex_ey_high_contrast.csv"
    "${ROOT}/data/output/validation/fig7a_triangular_low_contrast.csv"
    "${ROOT}/data/output/validation/fig7b_triangular_high_contrast.csv"
)

foreach(CSV IN LISTS CSV_FILES)
    if(NOT EXISTS "${CSV}")
        message(FATAL_ERROR "missing validation CSV: ${CSV}")
    endif()

    file(READ "${CSV}" CONTENT LIMIT 512)
    string(REGEX MATCH "^[^\n\r]*" HEADER "${CONTENT}")
    if(NOT HEADER STREQUAL EXPECTED)
        message(FATAL_ERROR "unexpected header in ${CSV}: ${HEADER}")
    endif()
endforeach()
