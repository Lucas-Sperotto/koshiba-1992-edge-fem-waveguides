#include "koshiba/mesh/mesh.hpp"

#include <algorithm>
#include <cmath>
#include <map>
#include <stdexcept>
#include <string>

namespace koshiba::mesh {

Edge::Edge(std::size_t id, std::size_t first_node_id, std::size_t second_node_id)
    : id_(id),
      first_node_id_(first_node_id),
      second_node_id_(second_node_id) {
    if (first_node_id_ == second_node_id_) {
        throw std::invalid_argument("edge endpoints must be distinct");
    }
}

std::size_t Edge::id() const noexcept {
    return id_;
}

std::size_t Edge::first_node_id() const noexcept {
    return first_node_id_;
}

std::size_t Edge::second_node_id() const noexcept {
    return second_node_id_;
}

bool Edge::is_boundary() const noexcept {
    return boundary_;
}

int Edge::physical_tag() const noexcept {
    return physical_tag_;
}

void Edge::mark_boundary(int physical_tag) noexcept {
    boundary_ = true;
    if (physical_tag != 0) {
        physical_tag_ = physical_tag;
    }
}

double Edge::length(const Node& first, const Node& second) const {
    const double dx = second.x - first.x;
    const double dy = second.y - first.y;
    return std::sqrt(dx * dx + dy * dy);
}

Point Edge::tangent(const Node& first, const Node& second) const {
    const double edge_length = length(first, second);
    if (edge_length == 0.0) {
        throw std::invalid_argument("edge tangent is undefined for zero length edge");
    }

    return {
        (second.x - first.x) / edge_length,
        (second.y - first.y) / edge_length,
    };
}

std::pair<std::size_t, std::size_t> Edge::canonical_node_ids(
    std::size_t first_node_id,
    std::size_t second_node_id) noexcept {
    return std::minmax(first_node_id, second_node_id);
}

Mesh::Mesh(std::vector<Node> nodes,
           std::vector<TriangleElement> elements,
           std::vector<BoundarySegment> boundary_segments,
           double tolerance)
    : nodes_(std::move(nodes)), elements_(std::move(elements)) {
    build_node_index();
    validate_elements(tolerance);
    build_edges(boundary_segments);
}

const std::vector<Node>& Mesh::nodes() const noexcept {
    return nodes_;
}

const std::vector<TriangleElement>& Mesh::elements() const noexcept {
    return elements_;
}

const std::vector<Edge>& Mesh::edges() const noexcept {
    return edges_;
}

const std::vector<ElementEdgeConnectivity>& Mesh::element_edges() const noexcept {
    return element_edges_;
}

const Node& Mesh::node_by_id(std::size_t node_id) const {
    return nodes_.at(node_index(node_id));
}

std::size_t Mesh::node_index(std::size_t node_id) const {
    const auto found = node_id_to_index_.find(node_id);
    if (found == node_id_to_index_.end()) {
        throw std::out_of_range("unknown node id " + std::to_string(node_id));
    }
    return found->second;
}

Triangle Mesh::triangle(std::size_t element_index) const {
    const auto& element = elements_.at(element_index);
    return Triangle({{
        node_by_id(element.node_ids[0]),
        node_by_id(element.node_ids[1]),
        node_by_id(element.node_ids[2]),
    }});
}

void Mesh::build_node_index() {
    for (std::size_t index = 0; index < nodes_.size(); ++index) {
        const auto [_, inserted] = node_id_to_index_.emplace(nodes_[index].id, index);
        if (!inserted) {
            throw std::invalid_argument("duplicate node id " + std::to_string(nodes_[index].id));
        }
    }
}

void Mesh::validate_elements(double tolerance) const {
    for (const auto& element : elements_) {
        if (element.node_ids[0] == element.node_ids[1] ||
            element.node_ids[1] == element.node_ids[2] ||
            element.node_ids[2] == element.node_ids[0]) {
            throw std::invalid_argument("triangle element has repeated node ids");
        }

        const Triangle triangle({{
            node_by_id(element.node_ids[0]),
            node_by_id(element.node_ids[1]),
            node_by_id(element.node_ids[2]),
        }});

        if (triangle.is_degenerate(tolerance)) {
            throw std::invalid_argument("degenerate triangle element " + std::to_string(element.id));
        }

        if (!triangle.is_counter_clockwise()) {
            throw std::invalid_argument("clockwise triangle element " + std::to_string(element.id));
        }
    }
}

void Mesh::build_edges(const std::vector<BoundarySegment>& boundary_segments) {
    constexpr std::array<std::array<int, 2>, 3> local_edge_nodes{{
        {{0, 1}},
        {{1, 2}},
        {{2, 0}},
    }};

    std::map<std::pair<std::size_t, std::size_t>, std::size_t> edge_ids_by_nodes;
    std::vector<std::size_t> incidence_counts;
    element_edges_.clear();

    for (const auto& element : elements_) {
        ElementEdgeConnectivity connectivity{};

        for (std::size_t local_edge = 0; local_edge < local_edge_nodes.size(); ++local_edge) {
            const auto [local_first, local_second] = local_edge_nodes[local_edge];
            const std::size_t first_node_id =
                element.node_ids[static_cast<std::size_t>(local_first)];
            const std::size_t second_node_id =
                element.node_ids[static_cast<std::size_t>(local_second)];
            const auto canonical = Edge::canonical_node_ids(first_node_id, second_node_id);

            auto found = edge_ids_by_nodes.find(canonical);
            if (found == edge_ids_by_nodes.end()) {
                const std::size_t edge_id = edges_.size();
                edges_.emplace_back(edge_id, canonical.first, canonical.second);
                incidence_counts.push_back(0);
                found = edge_ids_by_nodes.emplace(canonical, edge_id).first;
            }

            const std::size_t edge_id = found->second;
            connectivity.edge_ids[local_edge] = edge_id;
            connectivity.signs[local_edge] =
                first_node_id == canonical.first && second_node_id == canonical.second ? 1 : -1;
            ++incidence_counts[edge_id];
        }

        element_edges_.push_back(connectivity);
    }

    for (std::size_t edge_id = 0; edge_id < edges_.size(); ++edge_id) {
        if (incidence_counts[edge_id] == 1) {
            edges_[edge_id].mark_boundary(0);
        }
    }

    for (const auto& segment : boundary_segments) {
        const auto canonical =
            Edge::canonical_node_ids(segment.first_node_id, segment.second_node_id);
        const auto found = edge_ids_by_nodes.find(canonical);
        if (found == edge_ids_by_nodes.end()) {
            throw std::invalid_argument("boundary segment does not match a mesh edge");
        }

        edges_[found->second].mark_boundary(segment.physical_tag);
    }
}

}  // namespace koshiba::mesh
