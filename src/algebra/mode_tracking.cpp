#include "koshiba/algebra/mode_tracking.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace koshiba::algebra {

std::vector<Eigen::Index> match_modes_by_overlap(const Eigen::MatrixXd& previous_modes,
                                                 const Eigen::MatrixXd& current_modes) {
    if (previous_modes.rows() != current_modes.rows()) {
        throw std::invalid_argument("mode matrices must have the same row count");
    }

    const Eigen::Index previous_count = previous_modes.cols();
    const Eigen::Index current_count = current_modes.cols();
    std::vector<Eigen::Index> matches(static_cast<std::size_t>(previous_count), -1);
    std::vector<bool> used(static_cast<std::size_t>(current_count), false);

    for (Eigen::Index previous = 0; previous < previous_count; ++previous) {
        double best_overlap = -1.0;
        Eigen::Index best_current = -1;
        const double previous_norm = previous_modes.col(previous).norm();
        if (previous_norm == 0.0) {
            continue;
        }

        for (Eigen::Index current = 0; current < current_count; ++current) {
            if (used[static_cast<std::size_t>(current)]) {
                continue;
            }
            const double current_norm = current_modes.col(current).norm();
            if (current_norm == 0.0) {
                continue;
            }

            const double overlap =
                std::abs(previous_modes.col(previous).dot(current_modes.col(current))) /
                (previous_norm * current_norm);
            if (overlap > best_overlap) {
                best_overlap = overlap;
                best_current = current;
            }
        }

        if (best_current >= 0) {
            matches[static_cast<std::size_t>(previous)] = best_current;
            used[static_cast<std::size_t>(best_current)] = true;
        }
    }

    return matches;
}

}  // namespace koshiba::algebra
