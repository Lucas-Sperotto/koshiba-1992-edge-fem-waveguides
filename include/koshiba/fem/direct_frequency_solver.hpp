#pragma once

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <cstddef>
#include <string>
#include <vector>

#include "koshiba/fem/beta_solver.hpp"
#include "koshiba/mesh/mesh.hpp"
#include "koshiba/physics/material.hpp"

namespace koshiba::fem {

struct DirectFrequencyMatrices {
    Eigen::SparseMatrix<double> stiffness;
    Eigen::SparseMatrix<double> mass;
};

struct DirectFrequencyMode {
    std::size_t mode{};
    double k0_squared{};
    double k0{};
    double frequency_ghz{};
    std::string status{};
};

struct DirectFrequencyResult {
    std::vector<DirectFrequencyMode> modes{};
    Eigen::MatrixXd eigenvectors{};
    physics::FieldKind field_kind{physics::FieldKind::Electric};
    double beta{};
    std::size_t free_dofs{};
};

DirectFrequencyMatrices assemble_direct_frequency_matrices(
    const mesh::Mesh& mesh,
    const physics::MaterialMap& materials,
    physics::FieldKind field_kind,
    double beta);

DirectFrequencyResult solve_direct_frequency_modes(
    const mesh::Mesh& mesh,
    const physics::MaterialMap& materials,
    physics::FieldKind field_kind,
    double beta,
    const BetaSolverOptions& options = {});

}  // namespace koshiba::fem
