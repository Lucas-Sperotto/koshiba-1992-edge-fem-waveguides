#pragma once

#include <string>

#include "koshiba/mesh/mesh.hpp"

namespace koshiba::io {

mesh::Mesh read_gmsh_msh41_ascii(const std::string& path);

}  // namespace koshiba::io
