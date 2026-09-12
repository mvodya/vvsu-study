#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
REPO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd -P)"
BUILD_DIR="$SCRIPT_DIR/build"
LABS_DIR="${CPP_COURSE_LABS_DIR:-$REPO_ROOT/labs}"
RUN_GENERATORS=0
BUILD_PDF=0

for ARGUMENT in "$@"; do
    case "$ARGUMENT" in
        --generate)
            RUN_GENERATORS=1
            ;;
        --pdf)
            BUILD_PDF=1
            ;;
        *)
            printf 'Unknown argument: %s\n' "$ARGUMENT" >&2
            printf 'Usage: %s [--generate] [--pdf]\n' "$0" >&2
            exit 1
            ;;
    esac
done

cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" -DCPP_COURSE_LABS_DIR="$LABS_DIR"
cmake --build "$BUILD_DIR"

if [[ "$RUN_GENERATORS" -eq 1 ]]; then
    "$BUILD_DIR/generators/01-cpp-foundations/generate_lab_01_data" "$LABS_DIR/01-cpp-foundations"
    "$BUILD_DIR/generators/03-lifetime-raii/generate_lab_03_data" "$LABS_DIR/03-lifetime-raii"
    "$BUILD_DIR/generators/04-polymorphism/generate_lab_04_data" "$LABS_DIR/04-polymorphism"
    "$BUILD_DIR/generators/05-function-templates/generate_lab_05_data" "$LABS_DIR/05-function-templates"
    "$BUILD_DIR/generators/07-error-handling/generate_lab_07_data" "$LABS_DIR/07-error-handling"
    "$BUILD_DIR/generators/08-sequence-containers/generate_lab_08_data" "$LABS_DIR/08-sequence-containers"
    "$BUILD_DIR/generators/09-associative-containers/generate_lab_09_data" "$LABS_DIR/09-associative-containers"
    "$BUILD_DIR/generators/10-algorithms-lambdas/generate_lab_10_data" "$LABS_DIR/10-algorithms-lambdas"
    "$BUILD_DIR/generators/11-iterators-ranges/generate_lab_11_data" "$LABS_DIR/11-iterators-ranges"
    "$BUILD_DIR/generators/15-behavioral-patterns/generate_lab_15_data" "$LABS_DIR/15-behavioral-patterns"
    "$BUILD_DIR/generators/16-structural-patterns/generate_lab_16_data" "$LABS_DIR/16-structural-patterns"
fi

if [[ "$BUILD_PDF" -eq 1 ]]; then
    "$BUILD_DIR/md-to-pdf/md_to_pdf" "$LABS_DIR" "$SCRIPT_DIR/md-to-pdf/lab-template.typ" "$BUILD_DIR/md-to-pdf/typst" --compile --root "$REPO_ROOT"
fi
