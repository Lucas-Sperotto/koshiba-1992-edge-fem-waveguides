#include "koshiba/fem/beta_solver.hpp"
#include "koshiba/io/gmsh.hpp"
#include "koshiba/physics/material.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
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

std::vector<int> get_tags(const std::map<std::string, std::string>& config,
                          const std::string& key) {
    const auto found = config.find(key);
    if (found == config.end() || found->second.empty()) {
        return {};
    }

    std::vector<int> tags;
    std::stringstream stream(found->second);
    std::string item;
    while (std::getline(stream, item, ',')) {
        tags.push_back(std::stoi(trim(item)));
    }
    return tags;
}

bool get_bool(const std::map<std::string, std::string>& config,
              const std::string& key,
              bool fallback) {
    const auto found = config.find(key);
    if (found == config.end()) {
        return fallback;
    }
    return found->second == "true" || found->second == "1" || found->second == "yes";
}

void write_csv(const std::string& path, const koshiba::fem::BetaSolveResult& result) {
    const std::filesystem::path output_path(path);
    if (output_path.has_parent_path()) {
        std::filesystem::create_directories(output_path.parent_path());
    }

    std::ofstream output(path);
    if (!output) {
        throw std::runtime_error("unable to write output " + path);
    }

    output << "mode,beta2,beta,k0,field_kind,status\n";
    for (const auto& mode : result.modes) {
        output << mode.mode << ','
               << mode.beta2 << ','
               << mode.beta << ','
               << result.k0 << ','
               << koshiba::physics::to_string(result.field_kind) << ','
               << mode.status << '\n';
    }
}

}  // namespace

int main(int argc, char** argv) {
    try {
        if (argc != 3 || std::string(argv[1]) != "--input") {
            std::cerr << "usage: koshiba_mini_case --input <config.txt>\n";
            return 2;
        }

        const auto config = read_config(argv[2]);
        const auto mesh = koshiba::io::read_gmsh_msh41_ascii(required(config, "mesh"));
        const auto field_kind =
            koshiba::physics::parse_field_kind(required(config, "field_kind").c_str());
        const koshiba::physics::DiagonalMaterial material{
            get_double(config, "eps_rx", 1.0),
            get_double(config, "eps_ry", 1.0),
            get_double(config, "eps_rz", 1.0),
        };

        koshiba::fem::BetaSolverOptions options;
        options.requested_modes = get_size(config, "modes", 4);
        options.constraints.edge_physical_tags = get_tags(config, "constrained_edge_tags");
        options.constraints.constrain_nodes_touching_edges =
            get_bool(config, "constrain_nodes_touching_edges", false);

        const auto result = koshiba::fem::solve_beta_modes(
            mesh, material, field_kind, get_double(config, "k0", 1.0), options);
        write_csv(required(config, "output"), result);
    } catch (const std::exception& error) {
        std::cerr << "koshiba_mini_case: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
