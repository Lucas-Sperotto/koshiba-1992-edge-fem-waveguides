#pragma once

#include <Eigen/Dense>

#include <vector>

namespace koshiba::algebra {

std::vector<Eigen::Index> match_modes_by_overlap(const Eigen::MatrixXd& previous_modes,
                                                 const Eigen::MatrixXd& current_modes);

}  // namespace koshiba::algebra
