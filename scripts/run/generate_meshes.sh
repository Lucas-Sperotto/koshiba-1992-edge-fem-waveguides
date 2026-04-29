#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
MESH_DIR="$ROOT/out/meshes"

mkdir -p "$MESH_DIR"

if ! command -v gmsh >/dev/null 2>&1; then
    echo "generate_meshes: gmsh not found in PATH" >&2
    exit 1
fi

gmsh -2 -format msh41 \
    "$ROOT/examples/microstrip/fig3_microstrip.geo" \
    -o "$MESH_DIR/fig3_microstrip.msh"

gmsh -2 -format msh41 \
    "$ROOT/examples/rectangular_dielectric_waveguide/fig5_rectangular_dielectric.geo" \
    -o "$MESH_DIR/fig5_rectangular_dielectric.msh"

gmsh -2 -format msh41 \
    "$ROOT/examples/triangular_core_waveguide/fig7_triangular_core.geo" \
    -o "$MESH_DIR/fig7_triangular_core.msh"

echo "Meshes written to $MESH_DIR"
