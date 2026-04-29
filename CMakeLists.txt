cmake_minimum_required(VERSION 3.16)

project(koshiba_edge_fem_waveguides LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 REQUIRED)
find_program(GMSH_EXECUTABLE gmsh)
find_program(BASH_EXECUTABLE bash)

add_library(koshiba_fem
    src/algebra/mode_tracking.cpp
    src/algebra/reduced_eigenproblem.cpp
    src/fem/beta_solver.cpp
    src/fem/direct_frequency_solver.cpp
    src/fem/edge_shape.cpp
    src/fem/global_assembly.cpp
    src/fem/local_integrals.cpp
    src/io/gmsh.cpp
    src/mesh/mesh.cpp
    src/physics/material.cpp
    src/physics/normalization.cpp
    src/mesh/triangle.cpp
    src/validation/validation_quantities.cpp
)

target_include_directories(koshiba_fem
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(koshiba_fem PUBLIC cxx_std_17)

target_link_libraries(koshiba_fem PUBLIC Eigen3::Eigen)

if(MSVC)
    target_compile_options(koshiba_fem PRIVATE /W4)
else()
    target_compile_options(koshiba_fem PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

add_executable(koshiba_triangle_geometry_tests
    tests/triangle_geometry_tests.cpp
)

target_link_libraries(koshiba_triangle_geometry_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME triangle_geometry
    COMMAND koshiba_triangle_geometry_tests
)

add_executable(koshiba_mesh_connectivity_tests
    tests/mesh_connectivity_tests.cpp
)

target_link_libraries(koshiba_mesh_connectivity_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME mesh_connectivity
    COMMAND koshiba_mesh_connectivity_tests
)

add_executable(koshiba_gmsh_reader_tests
    tests/gmsh_reader_tests.cpp
)

target_link_libraries(koshiba_gmsh_reader_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME gmsh_reader
    COMMAND koshiba_gmsh_reader_tests
)

set_tests_properties(gmsh_reader PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(koshiba_fem_shape_integral_tests
    tests/fem_shape_integral_tests.cpp
)

target_link_libraries(koshiba_fem_shape_integral_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME fem_shape_integrals
    COMMAND koshiba_fem_shape_integral_tests
)

add_executable(koshiba_global_assembly_tests
    tests/global_assembly_tests.cpp
)

target_link_libraries(koshiba_global_assembly_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME global_assembly
    COMMAND koshiba_global_assembly_tests
)

add_executable(koshiba_reduced_eigenproblem_tests
    tests/reduced_eigenproblem_tests.cpp
)

target_link_libraries(koshiba_reduced_eigenproblem_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME reduced_eigenproblem
    COMMAND koshiba_reduced_eigenproblem_tests
)

add_executable(assembly_and_solver_tests
    tests/assembly_and_solver_tests.cpp
)

target_link_libraries(assembly_and_solver_tests
    PRIVATE
        koshiba_fem
)

add_test(NAME assembly_and_solver_tests COMMAND assembly_and_solver_tests)

set_tests_properties(assembly_and_solver_tests PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(koshiba_mini_case
    app/mini_case.cpp
)

target_link_libraries(koshiba_mini_case
    PRIVATE
        koshiba_fem
)

add_executable(koshiba_validation_case
    app/validation_case.cpp
)

target_link_libraries(koshiba_validation_case
    PRIVATE
        koshiba_fem
)

add_test(
    NAME mini_case_smoke
    COMMAND koshiba_mini_case --input data/input/mini_case.cfg
)

set_tests_properties(mini_case_smoke PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(koshiba_beta_solver_tests
    tests/beta_solver_tests.cpp
)

target_link_libraries(koshiba_beta_solver_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME beta_solver
    COMMAND koshiba_beta_solver_tests
)

set_tests_properties(beta_solver PROPERTIES
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(koshiba_material_region_and_direct_solver_tests
    tests/material_region_and_direct_solver_tests.cpp
)

target_link_libraries(koshiba_material_region_and_direct_solver_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME material_region_and_direct_solver
    COMMAND koshiba_material_region_and_direct_solver_tests
)

add_executable(koshiba_validation_quantities_tests
    tests/validation_quantities_tests.cpp
)

target_link_libraries(koshiba_validation_quantities_tests
    PRIVATE
        koshiba_fem
)

add_test(
    NAME validation_quantities
    COMMAND koshiba_validation_quantities_tests
)

add_test(
    NAME mini_case_csv_header
    COMMAND ${CMAKE_COMMAND}
        -DCSV=${CMAKE_CURRENT_SOURCE_DIR}/data/output/mini_case_modes.csv
        -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_csv_header.cmake
)

set_tests_properties(mini_case_csv_header PROPERTIES
    DEPENDS mini_case_smoke
)

if(GMSH_EXECUTABLE AND BASH_EXECUTABLE)
    add_test(
        NAME validation_pipeline_smoke
        COMMAND ${BASH_EXECUTABLE} scripts/run/run_all_validation.sh
    )

    set_tests_properties(validation_pipeline_smoke PROPERTIES
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_test(
        NAME validation_csv_headers
        COMMAND ${CMAKE_COMMAND}
            -DROOT=${CMAKE_CURRENT_SOURCE_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_validation_csv_headers.cmake
    )

    set_tests_properties(validation_csv_headers PROPERTIES
        DEPENDS validation_pipeline_smoke
    )

    add_test(
        NAME validation_fig3_units
        COMMAND ${CMAKE_COMMAND}
            -DROOT=${CMAKE_CURRENT_SOURCE_DIR}
            -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/check_validation_fig3_units.cmake
    )

    set_tests_properties(validation_fig3_units PROPERTIES
        DEPENDS validation_pipeline_smoke
    )
endif()
