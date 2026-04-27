cmake_minimum_required(VERSION 3.16)

project(koshiba_edge_fem_waveguides LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Eigen3 REQUIRED)

add_library(koshiba_fem
    src/algebra/reduced_eigenproblem.cpp
    src/fem/edge_shape.cpp
    src/fem/global_assembly.cpp
    src/fem/local_integrals.cpp
    src/io/gmsh.cpp
    src/mesh/mesh.cpp
    src/mesh/triangle.cpp
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
