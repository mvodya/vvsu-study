#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
BUILD_DIR="$SCRIPT_DIR/build"
LABS_DIR="${CPP_COURSE_LABS_DIR:-$REPO_ROOT/labs}"
RUN_GENERATORS=0

for ARGUMENT in "$@"; do
    case "$ARGUMENT" in
        --generate)
            RUN_GENERATORS=1
            ;;
        *)
            printf 'Unknown argument: %s\n' "$ARGUMENT" >&2
            printf 'Usage: %s [--generate]\n' "$0" >&2
            exit 1
            ;;
    esac
done

cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DCPP_COURSE_LABS_DIR="$LABS_DIR"
cmake --build "$BUILD_DIR"

if [[ "$RUN_GENERATORS" -eq 1 ]]; then
    "$BUILD_DIR/generators/01-cpp-foundations/generate_lab_01_data" "$LABS_DIR/01-cpp-foundations"
    "$BUILD_DIR/generators/03-lifetime-raii/generate_lab_03_data" "$LABS_DIR/03-lifetime-raii"
fi
