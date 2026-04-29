#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"

cmake -S "$ROOT" -B "$ROOT/build"
cmake --build "$ROOT/build"

"$ROOT/scripts/run/generate_meshes.sh"

CONFIGS=(
    "$ROOT/examples/microstrip/fig3a.cfg"
    "$ROOT/examples/microstrip/fig3b.cfg"
    "$ROOT/examples/rectangular_dielectric_waveguide/fig5a.cfg"
    "$ROOT/examples/rectangular_dielectric_waveguide/fig5b.cfg"
    "$ROOT/examples/rectangular_dielectric_waveguide/fig5c.cfg"
    "$ROOT/examples/triangular_core_waveguide/fig7a.cfg"
    "$ROOT/examples/triangular_core_waveguide/fig7b.cfg"
)

for config in "${CONFIGS[@]}"; do
    echo "Running validation config: ${config#$ROOT/}"
    (cd "$ROOT" && "$ROOT/build/koshiba_validation_case" --input "${config#$ROOT/}")
done

echo "Validation CSVs written to data/output/validation"
