#include "koshiba/mesh/mesh.hpp"

#include <array>
#include <cassert>
#include <stdexcept>
#include <vector>

namespace {

using koshiba::mesh::BoundarySegment;
using koshiba::mesh::Mesh;
using koshiba::mesh::Node;
using koshiba::mesh::TriangleElement;

std::vector<Node> nodes() {
    return {
        {1, 0.0, 0.0},
        {2, 1.0, 0.0},
        {3, 0.0, 1.0},
        {4, 1.0, 1.0},
    };
}

Mesh two_triangle_mesh() {
    return Mesh(
        nodes(),
        {
            {1, {{1, 2, 3}}, 21},
            {2, {{2, 4, 3}}, 21},
        },
        {
            {1, 2, 11},
            {2, 4, 12},
            {3, 4, 13},
            {1, 3, 14},
        });
}

void test_single_triangle_mesh() {
    const Mesh mesh(
        {
            {1, 0.0, 0.0},
            {2, 1.0, 0.0},
            {3, 0.0, 1.0},
        },
        {{1, {{1, 2, 3}}, 7}});

    assert(mesh.nodes().size() == 3);
    assert(mesh.elements().size() == 1);
    assert(mesh.edges().size() == 3);
    const std::array<std::size_t, 3> expected_edge_ids{{0, 1, 2}};
    const std::array<int, 3> expected_signs{{1, 1, -1}};
    assert(mesh.element_edges()[0].edge_ids == expected_edge_ids);
    assert(mesh.element_edges()[0].signs == expected_signs);
    for (const auto& edge : mesh.edges()) {
        assert(edge.is_boundary());
    }
}

void test_shared_edge_connectivity() {
    const auto mesh = two_triangle_mesh();

    assert(mesh.nodes().size() == 4);
    assert(mesh.elements().size() == 2);
    assert(mesh.edges().size() == 5);

    const auto& first = mesh.element_edges()[0];
    const auto& second = mesh.element_edges()[1];
    assert(first.edge_ids[1] == second.edge_ids[2]);
    assert(first.signs[1] == 1);
    assert(second.signs[2] == -1);
    assert(!mesh.edges()[first.edge_ids[1]].is_boundary());

    std::size_t boundary_count = 0;
    for (const auto& edge : mesh.edges()) {
        if (edge.is_boundary()) {
            ++boundary_count;
        }
    }
    assert(boundary_count == 4);
}

void test_physical_tags() {
    const auto mesh = two_triangle_mesh();

    bool saw_tag_11 = false;
    bool saw_tag_12 = false;
    bool saw_tag_13 = false;
    bool saw_tag_14 = false;
    for (const auto& edge : mesh.edges()) {
        saw_tag_11 = saw_tag_11 || edge.physical_tag() == 11;
        saw_tag_12 = saw_tag_12 || edge.physical_tag() == 12;
        saw_tag_13 = saw_tag_13 || edge.physical_tag() == 13;
        saw_tag_14 = saw_tag_14 || edge.physical_tag() == 14;
    }

    assert(saw_tag_11);
    assert(saw_tag_12);
    assert(saw_tag_13);
    assert(saw_tag_14);
}

void test_rejects_clockwise_triangle() {
    bool threw = false;
    try {
        const Mesh mesh(
            {
                {1, 0.0, 0.0},
                {2, 1.0, 0.0},
                {3, 0.0, 1.0},
            },
            {{1, {{1, 3, 2}}, 0}});
        (void)mesh;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

void test_rejects_degenerate_triangle() {
    bool threw = false;
    try {
        const Mesh mesh(
            {
                {1, 0.0, 0.0},
                {2, 1.0, 1.0},
                {3, 2.0, 2.0},
            },
            {{1, {{1, 2, 3}}, 0}});
        (void)mesh;
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);
}

}  // namespace

int main() {
    test_single_triangle_mesh();
    test_shared_edge_connectivity();
    test_physical_tags();
    test_rejects_clockwise_triangle();
    test_rejects_degenerate_triangle();
}
