#pragma once

#include <cstddef>
#include <utility>

#include "koshiba/mesh/node.hpp"
#include "koshiba/mesh/triangle.hpp"

namespace koshiba::mesh {

class Edge {
public:
    Edge() = default;
    Edge(std::size_t id, std::size_t first_node_id, std::size_t second_node_id);

    std::size_t id() const noexcept;
    std::size_t first_node_id() const noexcept;
    std::size_t second_node_id() const noexcept;

    bool is_boundary() const noexcept;
    int physical_tag() const noexcept;
    void mark_boundary(int physical_tag) noexcept;

    double length(const Node& first, const Node& second) const;
    Point tangent(const Node& first, const Node& second) const;

    static std::pair<std::size_t, std::size_t> canonical_node_ids(
        std::size_t first_node_id,
        std::size_t second_node_id) noexcept;

private:
    std::size_t id_{};
    std::size_t first_node_id_{};
    std::size_t second_node_id_{};
    bool boundary_{false};
    int physical_tag_{0};
};

}  // namespace koshiba::mesh
