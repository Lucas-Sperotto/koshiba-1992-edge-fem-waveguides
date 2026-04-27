#pragma once

#include <array>
#include <cstddef>
#include <unordered_map>
#include <vector>

#include "koshiba/mesh/edge.hpp"
#include "koshiba/mesh/node.hpp"
#include "koshiba/mesh/triangle.hpp"

namespace koshiba::mesh {

struct TriangleElement {
    std::size_t id{};
    std::array<std::size_t, 3> node_ids{};
    int physical_tag{};
};

struct BoundarySegment {
    std::size_t first_node_id{};
    std::size_t second_node_id{};
    int physical_tag{};
};

struct ElementEdgeConnectivity {
    std::array<std::size_t, 3> edge_ids{};
    std::array<int, 3> signs{};
};

class Mesh {
public:
    Mesh(std::vector<Node> nodes,
         std::vector<TriangleElement> elements,
         std::vector<BoundarySegment> boundary_segments = {},
         double tolerance = 1.0e-12);

    const std::vector<Node>& nodes() const noexcept;
    const std::vector<TriangleElement>& elements() const noexcept;
    const std::vector<Edge>& edges() const noexcept;
    const std::vector<ElementEdgeConnectivity>& element_edges() const noexcept;

    const Node& node_by_id(std::size_t node_id) const;
    std::size_t node_index(std::size_t node_id) const;
    Triangle triangle(std::size_t element_index) const;

private:
    void build_node_index();
    void validate_elements(double tolerance) const;
    void build_edges(const std::vector<BoundarySegment>& boundary_segments);

    std::vector<Node> nodes_{};
    std::vector<TriangleElement> elements_{};
    std::vector<Edge> edges_{};
    std::vector<ElementEdgeConnectivity> element_edges_{};
    std::unordered_map<std::size_t, std::size_t> node_id_to_index_{};
};

}  // namespace koshiba::mesh
