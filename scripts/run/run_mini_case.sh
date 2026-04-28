#!/usr/bin/env bash
set -euo pipefail

cmake -S . -B build
cmake --build build
./build/koshiba_mini_case --input data/input/mini_case.cfg
