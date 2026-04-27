#pragma once

#include <cstddef>

namespace koshiba::mesh {

struct Node {
    std::size_t id{};
    double x{};
    double y{};
};

}  // namespace koshiba::mesh
