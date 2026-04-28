#include "koshiba/fem/beta_solver.hpp"
#include "koshiba/io/gmsh.hpp"
#include "koshiba/physics/material.hpp"

#include <cassert>
#include <cmath>

namespace {

void test_material_coefficients() {
    const koshiba::physics::DiagonalMaterial material{2.0, 4.0, 8.0};

    const auto electric = material.coefficients(koshiba::physics::FieldKind::Electric);
    assert(electric.p_x == 1.0);
    assert(electric.p_y == 1.0);
    assert(electric.p_z == 1.0);
    assert(electric.q_x == 2.0);
    assert(electric.q_y == 4.0);
    assert(electric.q_z == 8.0);

    const auto magnetic = material.coefficients(koshiba::physics::FieldKind::Magnetic);
    assert(std::abs(magnetic.p_x - 0.5) < 1.0e-12);
    assert(std::abs(magnetic.p_y - 0.25) < 1.0e-12);
    assert(std::abs(magnetic.p_z - 0.125) < 1.0e-12);
    assert(magnetic.q_x == 1.0);
    assert(magnetic.q_y == 1.0);
    assert(magnetic.q_z == 1.0);
}

void test_beta_matrix_dimensions() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");
    const auto blocks = koshiba::fem::assemble_geometric_blocks(mesh);
    const koshiba::physics::DiagonalMaterial material{2.25, 2.25, 2.25};
    const auto coefficients = material.coefficients(koshiba::physics::FieldKind::Electric);
    const auto matrices = koshiba::fem::assemble_beta_matrices(blocks, coefficients, 4.0);

    assert(matrices.ktt_beta.rows() == 5);
    assert(matrices.ktt_beta.cols() == 5);
    assert(matrices.ktz_beta.rows() == 5);
    assert(matrices.ktz_beta.cols() == 4);
    assert(matrices.kzz_beta.rows() == 4);
    assert(matrices.kzz_beta.cols() == 4);
    assert(matrices.mtt_beta.rows() == 5);
    assert(matrices.mtt_beta.cols() == 5);
}

void test_beta_solver_returns_finite_modes() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");
    const koshiba::physics::DiagonalMaterial material{2.25, 2.25, 2.25};

    koshiba::fem::BetaSolverOptions options;
    options.requested_modes = 3;
    const auto result = koshiba::fem::solve_beta_modes(
        mesh, material, koshiba::physics::FieldKind::Electric, 4.0, options);

    assert(result.modes.size() == 3);
    assert(result.free_edge_dofs == 5);
    assert(result.free_node_dofs == 4);
    assert(result.effective_mass.rows() == 5);
    assert(result.effective_mass.cols() == 5);
    for (const auto& mode : result.modes) {
        assert(std::isfinite(mode.beta2));
    }
}

void test_boundary_constraints_remove_dofs() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");
    const koshiba::physics::DiagonalMaterial material{2.25, 2.25, 2.25};

    koshiba::fem::BetaSolverOptions options;
    options.requested_modes = 1;
    options.constraints.edge_physical_tags = {11};
    const auto result = koshiba::fem::solve_beta_modes(
        mesh, material, koshiba::physics::FieldKind::Magnetic, 4.0, options);

    assert(result.free_edge_dofs == 4);
    assert(result.free_node_dofs == 4);
    assert(!result.modes.empty());
}

void test_electric_pec_constraints_edges_and_nodes() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");
    const koshiba::physics::DiagonalMaterial material{2.25, 2.25, 2.25};

    koshiba::fem::BetaSolverOptions options;
    options.requested_modes = 1;
    options.constraints = koshiba::fem::essential_boundary_constraints(
        koshiba::physics::FieldKind::Electric,
        {{koshiba::fem::BoundaryConditionKind::PEC, {11}}});

    assert(options.constraints.edge_physical_tags.size() == 1);
    assert(options.constraints.edge_physical_tags[0] == 11);
    assert(options.constraints.node_physical_tags.size() == 1);
    assert(options.constraints.node_physical_tags[0] == 11);

    const auto result = koshiba::fem::solve_beta_modes(
        mesh, material, koshiba::physics::FieldKind::Electric, 4.0, options);

    assert(result.free_edge_dofs == 4);
    assert(result.free_node_dofs == 2);
    assert(!result.modes.empty());
}

void test_magnetic_pmc_constraints_edges_and_nodes() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");
    const koshiba::physics::DiagonalMaterial material{2.25, 2.25, 2.25};

    koshiba::fem::BetaSolverOptions options;
    options.requested_modes = 1;
    options.constraints = koshiba::fem::essential_boundary_constraints(
        koshiba::physics::FieldKind::Magnetic,
        {{koshiba::fem::BoundaryConditionKind::PMC, {12}}});

    const auto result = koshiba::fem::solve_beta_modes(
        mesh, material, koshiba::physics::FieldKind::Magnetic, 4.0, options);

    assert(result.free_edge_dofs == 4);
    assert(result.free_node_dofs == 2);
    assert(!result.modes.empty());
}

void test_cross_boundary_pairs_are_not_essential_constraints() {
    const auto electric_pmc = koshiba::fem::essential_boundary_constraints(
        koshiba::physics::FieldKind::Electric,
        {{koshiba::fem::BoundaryConditionKind::PMC, {11, 12}}});
    assert(electric_pmc.edge_physical_tags.empty());
    assert(electric_pmc.node_physical_tags.empty());

    const auto magnetic_pec = koshiba::fem::essential_boundary_constraints(
        koshiba::physics::FieldKind::Magnetic,
        {{koshiba::fem::BoundaryConditionKind::PEC, {11, 12}}});
    assert(magnetic_pec.edge_physical_tags.empty());
    assert(magnetic_pec.node_physical_tags.empty());
}

}  // namespace

int main() {
    test_material_coefficients();
    test_beta_matrix_dimensions();
    test_beta_solver_returns_finite_modes();
    test_boundary_constraints_remove_dofs();
    test_electric_pec_constraints_edges_and_nodes();
    test_magnetic_pmc_constraints_edges_and_nodes();
    test_cross_boundary_pairs_are_not_essential_constraints();
}
