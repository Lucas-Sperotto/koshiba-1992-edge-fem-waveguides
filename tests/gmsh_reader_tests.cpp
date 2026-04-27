#include "koshiba/io/gmsh.hpp"

#include <cassert>

namespace {

void test_reads_msh41_fixture() {
    const auto mesh = koshiba::io::read_gmsh_msh41_ascii(
        "tests/fixtures/two_triangles_msh41.msh");

    assert(mesh.nodes().size() == 4);
    assert(mesh.elements().size() == 2);
    assert(mesh.edges().size() == 5);
    assert(mesh.elements()[0].physical_tag == 21);
    assert(mesh.elements()[1].physical_tag == 21);

    std::size_t boundary_count = 0;
    bool saw_tag_11 = false;
    bool saw_tag_14 = false;
    for (const auto& edge : mesh.edges()) {
        if (edge.is_boundary()) {
            ++boundary_count;
        }
        saw_tag_11 = saw_tag_11 || edge.physical_tag() == 11;
        saw_tag_14 = saw_tag_14 || edge.physical_tag() == 14;
    }

    assert(boundary_count == 4);
    assert(saw_tag_11);
    assert(saw_tag_14);
}

}  // namespace

int main() {
    test_reads_msh41_fixture();
}
