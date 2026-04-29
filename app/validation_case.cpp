#include "koshiba/fem/beta_solver.hpp"
#include "koshiba/fem/direct_frequency_solver.hpp"
#include "koshiba/io/gmsh.hpp"
#include "koshiba/physics/normalization.hpp"
#include "koshiba/validation/validation_quantities.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace {

std::string trim(const std::string& value) {
    const auto first = value.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return {};
    }
    const auto last = value.find_last_not_of(" \t\r\n");
    return value.substr(first, last - first + 1);
}

std::map<std::string, std::string> read_config(const std::string& path) {
    std::ifstream input(path);
    if (!input) {
        throw std::runtime_error("unable to open config " + path);
    }

    std::map<std::string, std::string> config;
    std::string line;
    while (std::getline(input, line)) {
        const auto comment = line.find('#');
        if (comment != std::string::npos) {
            line = line.substr(0, comment);
        }
        line = trim(line);
        if (line.empty()) {
            continue;
        }

        const auto equals = line.find('=');
        if (equals == std::string::npos) {
            throw std::runtime_error("invalid config line: " + line);
        }
        config[trim(line.substr(0, equals))] = trim(line.substr(equals + 1));
    }
    return config;
}

std::string required(const std::map<std::string, std::string>& config,
                     const std::string& key) {
    const auto found = config.find(key);
    if (found == config.end()) {
        throw std::runtime_error("missing required config key " + key);
    }
    return found->second;
}

std::string get_string(const std::map<std::string, std::string>& config,
                       const std::string& key,
                       const std::string& fallback) {
    const auto found = config.find(key);
    return found == config.end() ? fallback : found->second;
}

double get_double(const std::map<std::string, std::string>& config,
                  const std::string& key,
                  double fallback) {
    const auto found = config.find(key);
    return found == config.end() ? fallback : std::stod(found->second);
}

std::size_t get_size(const std::map<std::string, std::string>& config,
                     const std::string& key,
                     std::size_t fallback) {
    const auto found = config.find(key);
    return found == config.end() ? fallback : static_cast<std::size_t>(std::stoul(found->second));
}

std::vector<std::string> split_csv(const std::string& value) {
    std::vector<std::string> items;
    std::stringstream stream(value);
    std::string item;
    while (std::getline(stream, item, ',')) {
        item = trim(item);
        if (!item.empty()) {
            items.push_back(item);
        }
    }
    return items;
}

std::vector<double> get_doubles(const std::map<std::string, std::string>& config,
                                const std::string& key) {
    std::vector<double> values;
    for (const auto& item : split_csv(required(config, key))) {
        values.push_back(std::stod(item));
    }
    if (values.empty()) {
        throw std::runtime_error("empty numeric list for key " + key);
    }
    return values;
}

std::vector<int> get_tags(const std::map<std::string, std::string>& config,
                          const std::string& key) {
    const auto found = config.find(key);
    if (found == config.end() || found->second.empty()) {
        return {};
    }

    std::vector<int> tags;
    for (const auto& item : split_csv(found->second)) {
        tags.push_back(std::stoi(item));
    }
    return tags;
}

koshiba::physics::DiagonalMaterial parse_material(const std::string& value) {
    const auto items = split_csv(value);
    if (items.size() != 3) {
        throw std::runtime_error("material entries must be eps_rx,eps_ry,eps_rz");
    }
    return {
        std::stod(items[0]),
        std::stod(items[1]),
        std::stod(items[2]),
    };
}

koshiba::physics::MaterialMap read_materials(
    const std::map<std::string, std::string>& config) {
    std::map<int, koshiba::physics::DiagonalMaterial> materials;
    const std::string prefix = "material.";
    for (const auto& [key, value] : config) {
        if (key.rfind(prefix, 0) != 0) {
            continue;
        }
        const std::string tag_text = key.substr(prefix.size());
        const int tag = tag_text == "default" ? 0 : std::stoi(tag_text);
        materials.emplace(tag, parse_material(value));
    }

    if (materials.empty()) {
        materials.emplace(0, koshiba::physics::DiagonalMaterial{
            get_double(config, "eps_rx", 1.0),
            get_double(config, "eps_ry", 1.0),
            get_double(config, "eps_rz", 1.0),
        });
    }
    return koshiba::physics::MaterialMap(std::move(materials));
}

bool contains(const std::vector<int>& values, int value) {
    for (int current : values) {
        if (current == value) {
            return true;
        }
    }
    return false;
}

void append_unique(std::vector<int>& target, const std::vector<int>& values) {
    for (int value : values) {
        if (!contains(target, value)) {
            target.push_back(value);
        }
    }
}

koshiba::fem::BetaSolverOptions read_options(
    const std::map<std::string, std::string>& config,
    koshiba::physics::FieldKind field_kind) {
    koshiba::fem::BetaSolverOptions options;
    options.requested_modes = get_size(config, "modes", 4);
    options.constraints.edge_physical_tags = get_tags(config, "constrained_edge_tags");
    options.constraints.node_physical_tags = get_tags(config, "constrained_node_tags");

    std::vector<koshiba::fem::PhysicalBoundaryCondition> boundary_conditions;
    const auto pec_tags = get_tags(config, "pec_tags");
    if (!pec_tags.empty()) {
        boundary_conditions.push_back({koshiba::fem::BoundaryConditionKind::PEC, pec_tags});
    }
    const auto pmc_tags = get_tags(config, "pmc_tags");
    if (!pmc_tags.empty()) {
        boundary_conditions.push_back({koshiba::fem::BoundaryConditionKind::PMC, pmc_tags});
    }
    const auto essential_constraints =
        koshiba::fem::essential_boundary_constraints(field_kind, boundary_conditions);
    append_unique(options.constraints.edge_physical_tags,
                  essential_constraints.edge_physical_tags);
    append_unique(options.constraints.node_physical_tags,
                  essential_constraints.node_physical_tags);
    return options;
}

std::vector<std::string> mode_labels(const std::map<std::string, std::string>& config,
                                     std::size_t count) {
    std::vector<std::string> labels = split_csv(get_string(config, "mode_labels", ""));
    while (labels.size() < count) {
        labels.push_back("mode_" + std::to_string(labels.size()));
    }
    return labels;
}

void ensure_parent(const std::string& path) {
    const std::filesystem::path output_path(path);
    if (output_path.has_parent_path()) {
        std::filesystem::create_directories(output_path.parent_path());
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 3 || std::string(argv[1]) != "--input") {
            std::cerr << "usage: koshiba_validation_case --input <config.txt>\n";
            return 2;
        }

        const auto config = read_config(argv[2]);
        const auto mesh = koshiba::io::read_gmsh_msh41_ascii(required(config, "mesh"));
        const auto materials = read_materials(config);
        const auto field_kind =
            koshiba::physics::parse_field_kind(required(config, "field_kind").c_str());
        const auto options = read_options(config, field_kind);
        const auto labels = mode_labels(config, options.requested_modes);

        const std::string output = required(config, "output");
        ensure_parent(output);
        std::ofstream csv(output);
        if (!csv) {
            throw std::runtime_error("unable to write output " + output);
        }

        const std::string case_id = required(config, "case_id");
        const std::string solve = required(config, "solve");
        csv << "case_id,curve_id,x,y,x_quantity,y_quantity,mode_label,field_kind,status\n";

        if (solve == "beta") {
            const std::string x_quantity = required(config, "x_quantity");
            const std::string y_quantity = get_string(config, "y_quantity", "b_number");
            const auto sweep = get_doubles(config, "sweep_values");
            const double scale_t_m = get_double(config, "scale_t_m", 1.0);
            const double n_core = get_double(config, "n_core", 1.0);
            const double n_cladding = get_double(config, "n_cladding", 1.0);

            for (double x : sweep) {
                double k0 = x;
                if (x_quantity == "v_number") {
                    k0 = koshiba::physics::k0_from_normalized_frequency(
                        x, scale_t_m, n_core, n_cladding);
                }

                const auto result =
                    koshiba::fem::solve_beta_modes(mesh, materials, field_kind, k0, options);
                for (const auto& mode : result.modes) {
                    double y = mode.beta;
                    std::string status = mode.status;
                    if (y_quantity == "b_number" && mode.status == "propagating") {
                        const auto classification =
                            koshiba::validation::classify_guided_b_from_beta2(
                                mode.beta2,
                                k0,
                                n_core,
                                n_cladding,
                                get_double(config, "guided_b_tolerance", 1.0e-10),
                                options.positive_tolerance);
                        y = classification.b;
                        status = classification.status;
                    }
                    csv << case_id << ','
                        << labels.at(mode.mode) << ','
                        << x << ','
                        << y << ','
                        << x_quantity << ','
                        << y_quantity << ','
                        << labels.at(mode.mode) << ','
                        << koshiba::physics::to_string(field_kind) << ','
                        << status << '\n';
                }
            }
        } else if (solve == "direct_frequency") {
            const auto sweep = get_doubles(config, "sweep_values");
            const std::string x_quantity = get_string(config, "x_quantity", "beta_rad_per_m");
            const std::string y_quantity = get_string(config, "y_quantity", "frequency_ghz");
            for (double x : sweep) {
                const double beta =
                    koshiba::validation::beta_rad_per_m_from_axis_value(x, x_quantity);
                const auto result = koshiba::fem::solve_direct_frequency_modes(
                    mesh, materials, field_kind, beta, options);
                for (const auto& mode : result.modes) {
                    csv << case_id << ','
                        << labels.at(mode.mode) << ','
                        << x << ','
                        << mode.frequency_ghz << ','
                        << x_quantity << ','
                        << y_quantity << ','
                        << labels.at(mode.mode) << ','
                        << koshiba::physics::to_string(field_kind) << ','
                        << mode.status << '\n';
                }
            }
        } else {
            throw std::runtime_error("unknown solve mode " + solve);
        }
    } catch (const std::exception& error) {
        std::cerr << "koshiba_validation_case: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
