#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <cstddef>
#include <string>
#include <vector>

#include "koshiba/fem/global_assembly.hpp"
#include "koshiba/mesh/mesh.hpp"
#include "koshiba/physics/material.hpp"

namespace koshiba::fem {

struct BetaMatrices {
    Eigen::SparseMatrix<double> ktt_beta;
    Eigen::SparseMatrix<double> ktz_beta;
    Eigen::SparseMatrix<double> kzz_beta;
    Eigen::SparseMatrix<double> mtt_beta;
};

enum class BoundaryConditionKind {
    PEC,
    PMC,
};

struct PhysicalBoundaryCondition {
    BoundaryConditionKind kind{BoundaryConditionKind::PEC};
    std::vector<int> physical_tags{};
};

struct BoundaryDofConstraints {
    std::vector<int> edge_physical_tags{};
    std::vector<int> node_physical_tags{};
    bool constrain_nodes_touching_edges{false};
};

struct BetaSolverOptions {
    std::size_t requested_modes{4};
    double positive_tolerance{1.0e-10};
    BoundaryDofConstraints constraints{};
};

struct BetaMode {
    std::size_t mode{};
    double beta2{};
    double beta{};
    std::string status{};
};

struct BetaSolveResult {
    std::vector<BetaMode> modes{};
    Eigen::MatrixXd transverse_eigenvectors{};
    Eigen::MatrixXd effective_mass{};
    physics::FieldKind field_kind{physics::FieldKind::Electric};
    double k0{};
    std::size_t free_edge_dofs{};
    std::size_t free_node_dofs{};
};

BetaMatrices assemble_beta_matrices(const GlobalAssemblyBlocks& blocks,
                                    const physics::DiagonalCoefficients& coefficients,
                                    double k0);

BetaMatrices assemble_beta_matrices(const mesh::Mesh& mesh,
                                    const physics::MaterialMap& materials,
                                    physics::FieldKind field_kind,
                                    double k0);

BoundaryDofConstraints essential_boundary_constraints(
    physics::FieldKind field_kind,
    const std::vector<PhysicalBoundaryCondition>& boundary_conditions);

BetaSolveResult solve_beta_modes(const mesh::Mesh& mesh,
                                 const physics::DiagonalMaterial& material,
                                 physics::FieldKind field_kind,
                                 double k0,
                                 const BetaSolverOptions& options = {});

BetaSolveResult solve_beta_modes(const mesh::Mesh& mesh,
                                 const physics::MaterialMap& materials,
                                 physics::FieldKind field_kind,
                                 double k0,
                                 const BetaSolverOptions& options = {});

}  // namespace koshiba::fem
