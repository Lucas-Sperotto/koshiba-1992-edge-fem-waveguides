#include "koshiba/algebra/mode_tracking.hpp"
#include "koshiba/fem/beta_solver.hpp"
#include "koshiba/fem/direct_frequency_solver.hpp"
#include "koshiba/fem/global_assembly.hpp"
#include "koshiba/mesh/mesh.hpp"
#include "koshiba/physics/normalization.hpp"

#include <Eigen/Sparse>

#include <cassert>
#include <cmath>
#include <map>

namespace {

koshiba::mesh::Mesh tagged_two_triangle_mesh() {
    return koshiba::mesh::Mesh(
        {
            {1, 0.0, 0.0},
            {2, 1.0, 0.0},
            {3, 0.0, 1.0},
            {4, 1.0, 1.0},
        },
        {
            {1, {{1, 2, 3}}, 21},
            {2, {{2, 4, 3}}, 22},
        },
        {
            {1, 2, 11},
            {2, 4, 12},
            {4, 3, 13},
            {3, 1, 14},
        });
}

double sparse_norm(const Eigen::SparseMatrix<double>& matrix) {
    return matrix.norm();
}

void test_homogeneous_material_map_matches_legacy_combination() {
    const auto mesh = tagged_two_triangle_mesh();
    const koshiba::physics::DiagonalMaterial material{2.25, 2.25, 2.25};
    const auto coefficients = material.coefficients(koshiba::physics::FieldKind::Electric);
    const auto geometric = koshiba::fem::assemble_geometric_blocks(mesh);
    const auto legacy = koshiba::fem::assemble_beta_matrices(geometric, coefficients, 4.0);
    const auto mapped = koshiba::fem::assemble_beta_matrices(
        mesh,
        koshiba::physics::MaterialMap::homogeneous(material),
        koshiba::physics::FieldKind::Electric,
        4.0);

    assert(sparse_norm(legacy.ktt_beta - mapped.ktt_beta) < 1.0e-12);
    assert(sparse_norm(legacy.ktz_beta - mapped.ktz_beta) < 1.0e-12);
    assert(sparse_norm(legacy.kzz_beta - mapped.kzz_beta) < 1.0e-12);
    assert(sparse_norm(legacy.mtt_beta - mapped.mtt_beta) < 1.0e-12);
}

void test_region_materials_change_assembled_matrices() {
    const auto mesh = tagged_two_triangle_mesh();
    const auto homogeneous = koshiba::fem::assemble_beta_matrices(
        mesh,
        koshiba::physics::MaterialMap::homogeneous({2.25, 2.25, 2.25}),
        koshiba::physics::FieldKind::Electric,
        4.0);
    const koshiba::physics::MaterialMap regional({
        {21, {2.25, 2.25, 2.25}},
        {22, {4.0, 4.0, 4.0}},
    });
    const auto heterogeneous = koshiba::fem::assemble_beta_matrices(
        mesh, regional, koshiba::physics::FieldKind::Electric, 4.0);

    assert(sparse_norm(homogeneous.ktt_beta - heterogeneous.ktt_beta) > 1.0e-6);
    assert(sparse_norm(homogeneous.kzz_beta - heterogeneous.kzz_beta) > 1.0e-6);
}

void test_missing_material_tag_throws() {
    const auto mesh = tagged_two_triangle_mesh();
    const koshiba::physics::MaterialMap incomplete({{21, {2.25, 2.25, 2.25}}});

    bool threw = false;
    try {
        (void)koshiba::fem::assemble_beta_matrices(
            mesh, incomplete, koshiba::physics::FieldKind::Electric, 4.0);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

void test_direct_frequency_solver_returns_finite_values() {
    const auto mesh = tagged_two_triangle_mesh();
    const koshiba::physics::MaterialMap materials({
        {21, {2.25, 2.25, 2.25}},
        {22, {1.0, 1.0, 1.0}},
    });
    koshiba::fem::BetaSolverOptions options;
    options.requested_modes = 3;

    const auto result = koshiba::fem::solve_direct_frequency_modes(
        mesh, materials, koshiba::physics::FieldKind::Electric, 1.0, options);

    assert(result.modes.size() == 3);
    assert(result.free_dofs == mesh.edges().size() + mesh.nodes().size());
    for (const auto& mode : result.modes) {
        assert(std::isfinite(mode.k0_squared));
    }
}

void test_normalization_helpers_round_trip() {
    const double k0 =
        koshiba::physics::k0_from_normalized_frequency(0.5, 2.0, 1.5, 1.0);
    const double v =
        koshiba::physics::normalized_frequency_from_k0(k0, 2.0, 1.5, 1.0);
    assert(std::abs(v - 0.5) < 1.0e-12);

    const double frequency = koshiba::physics::frequency_ghz_from_k0(k0);
    const double k0_round_trip = koshiba::physics::k0_from_frequency_ghz(frequency);
    assert(std::abs(k0 - k0_round_trip) < 1.0e-12);
}

void test_mode_tracking_by_overlap() {
    Eigen::MatrixXd previous(3, 2);
    previous << 1.0, 0.0,
                0.0, 1.0,
                0.0, 0.0;
    Eigen::MatrixXd current(3, 2);
    current << 0.0, 1.0,
               1.0, 0.0,
               0.0, 0.0;

    const auto matches = koshiba::algebra::match_modes_by_overlap(previous, current);
    assert(matches.size() == 2);
    assert(matches[0] == 1);
    assert(matches[1] == 0);
}

}  // namespace

int main() {
    test_homogeneous_material_map_matches_legacy_combination();
    test_region_materials_change_assembled_matrices();
    test_missing_material_tag_throws();
    test_direct_frequency_solver_returns_finite_values();
    test_normalization_helpers_round_trip();
    test_mode_tracking_by_overlap();
}
