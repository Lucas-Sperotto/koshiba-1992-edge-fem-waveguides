cmake_minimum_required(VERSION 3.16)

project(koshiba_edge_fem_waveguides LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(koshiba_fem
    src/mesh/triangle.cpp
)

target_include_directories(koshiba_fem
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(koshiba_fem PUBLIC cxx_std_17)

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
