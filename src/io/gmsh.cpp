#include "koshiba/io/gmsh.hpp"

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace koshiba::io {
namespace {

using EntityKey = std::pair<int, int>;

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::string collect_section(std::istream& input, const std::string& section_name) {
    std::ostringstream buffer;
    std::string line;
    const std::string end_marker = "$End" + section_name.substr(1);

    while (std::getline(input, line)) {
        if (trim(line) == end_marker) {
            return buffer.str();
        }
        buffer << line << '\n';
    }

    throw std::runtime_error("unterminated Gmsh section " + section_name);
}

int first_physical_tag(std::istream& input) {
    int physical_count = 0;
    input >> physical_count;
    int first_tag = 0;
    for (int i = 0; i < physical_count; ++i) {
        int tag = 0;
        input >> tag;
        if (i == 0) {
            first_tag = tag;
        }
    }
    return first_tag;
}

std::map<EntityKey, int> parse_entities(const std::string& content) {
    std::istringstream input(content);
    int point_count = 0;
    int curve_count = 0;
    int surface_count = 0;
    int volume_count = 0;
    input >> point_count >> curve_count >> surface_count >> volume_count;
    if (!input) {
        throw std::runtime_error("invalid $Entities section");
    }

    std::map<EntityKey, int> physical_tags;

    for (int i = 0; i < point_count; ++i) {
        int tag = 0;
        double x = 0.0;
        double y = 0.0;
        double z = 0.0;
        input >> tag >> x >> y >> z;
        physical_tags[{0, tag}] = first_physical_tag(input);
    }

    for (int i = 0; i < curve_count; ++i) {
        int tag = 0;
        double min_x = 0.0;
        double min_y = 0.0;
        double min_z = 0.0;
        double max_x = 0.0;
        double max_y = 0.0;
        double max_z = 0.0;
        input >> tag >> min_x >> min_y >> min_z >> max_x >> max_y >> max_z;
        physical_tags[{1, tag}] = first_physical_tag(input);

        int bounding_points = 0;
        input >> bounding_points;
        for (int j = 0; j < bounding_points; ++j) {
            int ignored = 0;
            input >> ignored;
        }
    }

    for (int i = 0; i < surface_count; ++i) {
        int tag = 0;
        double min_x = 0.0;
        double min_y = 0.0;
        double min_z = 0.0;
        double max_x = 0.0;
        double max_y = 0.0;
        double max_z = 0.0;
        input >> tag >> min_x >> min_y >> min_z >> max_x >> max_y >> max_z;
        physical_tags[{2, tag}] = first_physical_tag(input);

        int bounding_curves = 0;
        input >> bounding_curves;
        for (int j = 0; j < bounding_curves; ++j) {
            int ignored = 0;
            input >> ignored;
        }
    }

    for (int i = 0; i < volume_count; ++i) {
        int tag = 0;
        double min_x = 0.0;
        double min_y = 0.0;
        double min_z = 0.0;
        double max_x = 0.0;
        double max_y = 0.0;
        double max_z = 0.0;
        input >> tag >> min_x >> min_y >> min_z >> max_x >> max_y >> max_z;
        physical_tags[{3, tag}] = first_physical_tag(input);

        int bounding_surfaces = 0;
        input >> bounding_surfaces;
        for (int j = 0; j < bounding_surfaces; ++j) {
            int ignored = 0;
            input >> ignored;
        }
    }

    return physical_tags;
}

std::vector<mesh::Node> parse_nodes(const std::string& content) {
    std::istringstream input(content);
    std::size_t entity_block_count = 0;
    std::size_t node_count = 0;
    std::size_t min_node_tag = 0;
    std::size_t max_node_tag = 0;
    input >> entity_block_count >> node_count >> min_node_tag >> max_node_tag;
    if (!input) {
        throw std::runtime_error("invalid $Nodes section");
    }

    std::vector<mesh::Node> nodes;
    nodes.reserve(node_count);

    for (std::size_t block = 0; block < entity_block_count; ++block) {
        int entity_dim = 0;
        int entity_tag = 0;
        int parametric = 0;
        std::size_t nodes_in_block = 0;
        input >> entity_dim >> entity_tag >> parametric >> nodes_in_block;
        if (parametric != 0) {
            throw std::runtime_error("parametric Gmsh nodes are not supported");
        }

        std::vector<std::size_t> tags(nodes_in_block);
        for (auto& tag : tags) {
            input >> tag;
        }

        for (std::size_t i = 0; i < nodes_in_block; ++i) {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
            input >> x >> y >> z;
            nodes.push_back({tags[i], x, y});
        }
    }

    return nodes;
}

struct ParsedElements {
    std::vector<mesh::TriangleElement> triangles;
    std::vector<mesh::BoundarySegment> boundary_segments;
};

int physical_tag_for(const std::map<EntityKey, int>& physical_tags, int dim, int tag) {
    const auto found = physical_tags.find({dim, tag});
    return found == physical_tags.end() ? 0 : found->second;
}

ParsedElements parse_elements(const std::string& content,
                              const std::map<EntityKey, int>& physical_tags) {
    std::istringstream input(content);
    std::size_t entity_block_count = 0;
    std::size_t element_count = 0;
    std::size_t min_element_tag = 0;
    std::size_t max_element_tag = 0;
    input >> entity_block_count >> element_count >> min_element_tag >> max_element_tag;
    if (!input) {
        throw std::runtime_error("invalid $Elements section");
    }

    ParsedElements elements;

    for (std::size_t block = 0; block < entity_block_count; ++block) {
        int entity_dim = 0;
        int entity_tag = 0;
        int element_type = 0;
        std::size_t elements_in_block = 0;
        input >> entity_dim >> entity_tag >> element_type >> elements_in_block;

        if (entity_dim == 1 && element_type != 1) {
            throw std::runtime_error("only 2-node line boundary elements are supported");
        }
        if (entity_dim == 2 && element_type != 2) {
            throw std::runtime_error("only 3-node triangle surface elements are supported");
        }
        if (entity_dim > 2 || (entity_dim == 0 && element_type != 15)) {
            throw std::runtime_error("unsupported Gmsh element block");
        }

        const int physical_tag = physical_tag_for(physical_tags, entity_dim, entity_tag);

        for (std::size_t i = 0; i < elements_in_block; ++i) {
            std::size_t element_tag = 0;
            input >> element_tag;

            if (entity_dim == 1) {
                std::size_t n1 = 0;
                std::size_t n2 = 0;
                input >> n1 >> n2;
                elements.boundary_segments.push_back({n1, n2, physical_tag});
            } else if (entity_dim == 2) {
                std::size_t n1 = 0;
                std::size_t n2 = 0;
                std::size_t n3 = 0;
                input >> n1 >> n2 >> n3;
                elements.triangles.push_back({element_tag, {{n1, n2, n3}}, physical_tag});
            } else {
                std::size_t ignored = 0;
                input >> ignored;
            }
        }
    }

    return elements;
}

void parse_mesh_format(const std::string& content) {
    std::istringstream input(content);
    std::string version;
    int file_type = 0;
    int data_size = 0;
    input >> version >> file_type >> data_size;
    if (version != "4.1" || file_type != 0) {
        throw std::runtime_error("only Gmsh MSH 4.1 ASCII is supported");
    }
}

}  // namespace

mesh::Mesh read_gmsh_msh41_ascii(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("unable to open Gmsh file " + path);
    }

    std::map<EntityKey, int> physical_tags;
    std::vector<mesh::Node> nodes;
    ParsedElements parsed_elements;
    bool saw_mesh_format = false;
    bool saw_nodes = false;
    bool saw_elements = false;

    std::string line;
    while (std::getline(input, line)) {
        const std::string marker = trim(line);
        if (marker.empty()) {
            continue;
        }

        if (marker == "$MeshFormat") {
            parse_mesh_format(collect_section(input, marker));
            saw_mesh_format = true;
        } else if (marker == "$Entities") {
            physical_tags = parse_entities(collect_section(input, marker));
        } else if (marker == "$Nodes") {
            nodes = parse_nodes(collect_section(input, marker));
            saw_nodes = true;
        } else if (marker == "$Elements") {
            parsed_elements = parse_elements(collect_section(input, marker), physical_tags);
            saw_elements = true;
        } else if (marker == "$PhysicalNames") {
            (void)collect_section(input, marker);
        }
    }

    if (!saw_mesh_format || !saw_nodes || !saw_elements) {
        throw std::runtime_error("Gmsh file is missing MeshFormat, Nodes, or Elements section");
    }

    if (parsed_elements.triangles.empty()) {
        throw std::runtime_error("Gmsh file does not contain triangle elements");
    }

    return mesh::Mesh(
        std::move(nodes),
        std::move(parsed_elements.triangles),
        std::move(parsed_elements.boundary_segments));
}

}  // namespace koshiba::io
