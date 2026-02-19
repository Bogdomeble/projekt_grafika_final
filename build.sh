#!/usr/bin/env bash
# Concise build script for Art Gallery 3D Renderer
# - minimal output, builds project, copies assets to build/bin/assets
# Usage: ./build.sh [-d|--debug] [-j N] [-b DIR] [--clean] [-h|--help]

set -euo pipefail

PROG="$(basename "$0")"
BUILD_TYPE="Release"
BUILD_DIR="build"
JOBS="$(nproc 2>/dev/null || echo 4)"
CLEAN=0

usage() {
  cat <<EOF
Usage: $PROG [options]

Options:
  -d, --debug         Build Debug (default: Release)
  -j N, --jobs N      Number of parallel build jobs (default: auto-detect)
  -b DIR, --build-dir DIR  Build directory (default: build)
  --clean             Remove build directory and exit
  -h, --help          Show this help
EOF
}

# Parse args (simple)
while [[ $# -gt 0 ]]; do
  case "$1" in
    -d|--debug) BUILD_TYPE="Debug"; shift ;;
    -j|--jobs) JOBS="${2:-$JOBS}"; shift 2 ;;
    -b|--build-dir) BUILD_DIR="${2:-$BUILD_DIR}"; shift 2 ;;
    --clean) CLEAN=1; shift ;;
    -h|--help) usage; exit 0 ;;
    *) echo "Unknown option: $1"; usage; exit 2 ;;
  esac
done

if [[ "$CLEAN" -eq 1 ]]; then
  echo "Removing build directory: $BUILD_DIR"
  rm -rf "$BUILD_DIR"
  exit 0
fi

# Basic checks
command -v cmake >/dev/null 2>&1 || { echo "Error: cmake not found"; exit 1; }
command -v g++ >/dev/null 2>&1 || command -v clang++ >/dev/null 2>&1 || { echo "Error: C++ compiler not found"; exit 1; }

echo "Build: $BUILD_TYPE  Dir: $BUILD_DIR  Jobs: $JOBS"

# Prepare build dir
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

echo "Configuring..."
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" .. >/dev/null

echo "Building..."
cmake --build . --config "$BUILD_TYPE" -j "$JOBS"

# Copy assets if present
BIN_DIR="./bin"
ASSETS_SRC="../assets"
ASSETS_DST="$BIN_DIR/assets"

if [[ -d "$ASSETS_SRC" ]]; then
  mkdir -p "$ASSETS_DST"
  cp -a "$ASSETS_SRC/." "$ASSETS_DST/" || true
  echo "Assets copied to $ASSETS_DST"
else
  echo "Warning: top-level assets/ not found; runtime may fail if assets are missing"
fi

# Report
EXECUTABLE="$BIN_DIR/ArtGallery"
if [[ -f "$EXECUTABLE" ]]; then
  echo "Build succeeded. Executable: $EXECUTABLE"
else
  echo "Build completed. Executable not found at $EXECUTABLE"
fi

if [[ -d "$ASSETS_DST/shaders" ]]; then
  echo "Shaders: OK ($(find "$ASSETS_DST/shaders" -type f | wc -l) files)"
else
  echo "Shaders: missing in build output"
fi

if [[ -d "$ASSETS_DST/textures" ]]; then
  echo "Textures: OK ($(find "$ASSETS_DST/textures" -type f | wc -l) files)"
else
  echo "Textures: missing in build output"
fi

exit 0
