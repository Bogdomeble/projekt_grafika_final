#!/usr/bin/env bash
#
# Simplified build script for Art Gallery 3D Renderer
# - keeps core options: build type, jobs, build dir, sanitizer, verbose
# - improves dependency detection (GLFW, GLM, OpenGL)
# - detects/copies assets into build output (build/bin/assets)
#
# Usage:
#   ./build.sh                # Release build (auto-detect jobs)
#   ./build.sh -d             # Debug build
#   ./build.sh -j 8           # Use 8 parallel jobs
#   ./build.sh --clean        # Remove build directory and exit
#   ./build.sh --help         # Show help
#
set -euo pipefail

# Color helpers
_RED='\033[0;31m'
_GREEN='\033[0;32m'
_YELLOW='\033[1;33m'
_BLUE='\033[0;34m'
_NC='\033[0m'

# Defaults
BUILD_TYPE="Release"
BUILD_DIR="build"
ENABLE_SANITIZER=0
VERBOSE=0
JOBS="$(nproc 2>/dev/null || echo 4)"
SHOW_HELP=0

# Helper: print usage
usage() {
    cat <<EOF
Usage: $0 [options]

Options:
  -d, --debug            Build with Debug (default: Release)
  -r, --release          Build with Release
  -s, --sanitizer        Enable AddressSanitizer (only meaningful for Debug)
  -v, --verbose          Enable verbose build output
  -j, --jobs NUM         Number of parallel jobs (default: auto-detect)
  -b, --build-dir DIR    Build directory (default: build)
  -c, --clean            Remove build directory and exit
  -h, --help             Show this help
EOF
}

# Parse args
while [[ $# -gt 0 ]]; do
    case "$1" in
        -d|--debug) BUILD_TYPE="Debug"; shift ;;
        -r|--release) BUILD_TYPE="Release"; shift ;;
        -s|--sanitizer) ENABLE_SANITIZER=1; shift ;;
        -v|--verbose) VERBOSE=1; shift ;;
        -j|--jobs) JOBS="${2:-$JOBS}"; shift 2 ;;
        -b|--build-dir) BUILD_DIR="${2:-$BUILD_DIR}"; shift 2 ;;
        -c|--clean)
            echo -e "${_BLUE}Cleaning build directory: ${BUILD_DIR}${_NC}"
            rm -rf "${BUILD_DIR}"
            echo -e "${_GREEN}Done.${_NC}"
            exit 0
            ;;
        -h|--help) SHOW_HELP=1; shift ;;
        *)
            echo -e "${_RED}Unknown option: $1${_NC}"
            usage
            exit 2
            ;;
    esac
done

if [[ $SHOW_HELP -eq 1 ]]; then
    usage
    exit 0
fi

# Print configuration
echo -e "${_BLUE}================================${_NC}"
echo -e "${_BLUE}Art Gallery 3D Renderer${_NC}"
echo -e "${_BLUE}================================${_NC}"
echo -e "Build Type:        ${_YELLOW}${BUILD_TYPE}${_NC}"
echo -e "Build Directory:   ${_YELLOW}${BUILD_DIR}${_NC}"
echo -e "Parallel Jobs:     ${_YELLOW}${JOBS}${_NC}"
echo -e "Sanitizer:         ${_YELLOW}$([ "${ENABLE_SANITIZER}" -eq 1 ] && echo "ON" || echo "OFF")${_NC}"
echo -e "Verbose:           ${_YELLOW}$([ "${VERBOSE}" -eq 1 ] && echo "ON" || echo "OFF")${_NC}"
echo -e "${_BLUE}================================${_NC}"
echo ""

# Check common tools
if ! command -v cmake &>/dev/null; then
    echo -e "${_RED}Error: cmake is not installed. Install cmake 3.15+ and retry.${_NC}"
    exit 1
fi
echo -e "${_GREEN}✓${_NC} CMake found: $(cmake --version | head -n1)"

if command -v g++ &>/dev/null; then
    COMPILER="g++"
elif command -v clang++ &>/dev/null; then
    COMPILER="clang++"
else
    echo -e "${_RED}Error: No C++ compiler (g++/clang++) found.${_NC}"
    exit 1
fi
echo -e "${_GREEN}✓${_NC} Compiler found: ${COMPILER} ($( ${COMPILER} --version | head -n1 ))"
echo ""

# Dependency checks (informational; we try to be helpful)
echo -e "${_BLUE}Checking dependencies...${_NC}"

# GLFW: prefer pkg-config
if pkg-config --exists glfw3 2>/dev/null; then
    echo -e "${_GREEN}✓${_NC} GLFW3 found (pkg-config)"
else
    echo -e "${_YELLOW}⚠${_NC} GLFW3 not found via pkg-config. Ensure libglfw3-dev (or equivalent) is installed."
fi

# Attempt GLM detection: pkg-config OR header presence OR cmake package
GLM_PKG_OK=0
if pkg-config --exists glm 2>/dev/null; then
    echo -e "${_GREEN}✓${_NC} GLM found (pkg-config)"
    GLM_PKG_OK=1
else
    # Check for common include paths (header-only)
    if [[ -d "/usr/include/glm" ]] || [[ -d "/usr/local/include/glm" ]] || [[ -d "/opt/homebrew/include/glm" ]]; then
        echo -e "${_GREEN}✓${_NC} GLM headers found in standard include paths"
        GLM_PKG_OK=1
    else
        echo -e "${_YELLOW}⚠${_NC} GLM not detected. Install libglm-dev or provide glm to CMake."
        echo "  Ubuntu/Debian: sudo apt-get install libglm-dev"
        echo "  macOS: brew install glm"
    fi
fi

# OpenGL
if pkg-config --exists gl 2>/dev/null || pkg-config --exists glx 2>/dev/null; then
    echo -e "${_GREEN}✓${_NC} OpenGL dev files found (pkg-config)"
else
    echo -e "${_YELLOW}⚠${_NC} OpenGL development files not found via pkg-config. Install libgl1-mesa-dev (Linux) or ensure SDK on macOS."
fi

echo ""

# Prepare build directory
mkdir -p "${BUILD_DIR}"
cd "${BUILD_DIR}"

# Prepare CMake flags
CMAKE_FLAGS=("-DCMAKE_BUILD_TYPE=${BUILD_TYPE}")

if [[ "${ENABLE_SANITIZER}" -eq 1 && "${BUILD_TYPE}" == "Debug" ]]; then
    CMAKE_FLAGS+=("-DENABLE_SANITIZER=ON")
fi

if [[ "${VERBOSE}" -eq 1 ]]; then
    CMAKE_FLAGS+=("-DCMAKE_VERBOSE_MAKEFILE=ON")
fi

# If we detected GLM headers but pkg-config not present, optionally tell CMake where to find includes.
# Many projects expect a glmConfig.cmake; if not present, header-only path may suffice.
if [[ "${GLM_PKG_OK}" -eq 1 && ! pkg-config --exists glm 2>/dev/null ]]; then
    # Prefer /usr/local/include over /usr/include if both present
    if [[ -d "/usr/local/include/glm" ]]; then
        CMAKE_FLAGS+=("-DGLM_INCLUDE_DIR=/usr/local/include")
    elif [[ -d "/usr/include/glm" ]]; then
        CMAKE_FLAGS+=("-DGLM_INCLUDE_DIR=/usr/include")
    fi
fi

echo -e "${_BLUE}Configuring CMake...${_NC}"
echo "cmake ${CMAKE_FLAGS[*]} .."
if ! cmake "${CMAKE_FLAGS[@]}" ..; then
    echo -e "${_RED}CMake configuration failed. See output above for details.${_NC}"
    exit 1
fi
echo -e "${_GREEN}✓ CMake configuration complete${_NC}"
echo ""

# Build
echo -e "${_BLUE}Building project (${JOBS} jobs)...${_NC}"
BUILD_CMD=(cmake --build . --config "${BUILD_TYPE}" -j "${JOBS}")
if [[ "${VERBOSE}" -eq 1 ]]; then
    "${BUILD_CMD[@]}" --verbose
else
    "${BUILD_CMD[@]}"
fi
echo -e "${_GREEN}✓ Build complete!${_NC}"
echo ""

# Ensure output bin dir exists
BIN_DIR="./bin"
mkdir -p "${BIN_DIR}"

# Assets handling: if top-level assets/ exists, copy it into build/bin/assets
ROOT_DIR="$(cd .. >/dev/null 2>&1 && pwd -P)/$(basename "$(pwd)")"
# Note: The above resolves to the project root under normal invocation; keep it best-effort.
# Simpler check: look for ../assets relative to build dir
SOURCE_ASSETS="../assets"
TARGET_ASSETS="${BIN_DIR}/assets"

if [[ -d "${SOURCE_ASSETS}" ]]; then
    echo -e "${_BLUE}Copying assets to build output...${_NC}"
    rm -rf "${TARGET_ASSETS}"
    mkdir -p "${TARGET_ASSETS}"
    # copy while preserving structure
    cp -a "${SOURCE_ASSETS}/." "${TARGET_ASSETS}/"
    echo -e "${_GREEN}✓ Assets copied to ${TARGET_ASSETS}${_NC}"
else
    echo -e "${_YELLOW}⚠${_NC} No top-level 'assets/' directory found at ${SOURCE_ASSETS}; skipping asset copy."
fi

# Quick asset checks (shaders/textures) inside build/bin/assets
echo ""
echo -e "${_BLUE}Asset Status:${_NC}"
SHADERS_DIR="${TARGET_ASSETS}/shaders"
TEXTURES_DIR="${TARGET_ASSETS}/textures"

if [[ -d "${SHADERS_DIR}" ]]; then
    SHADER_COUNT=$(find "${SHADERS_DIR}" -type f | wc -l)
    echo -e "  ${_GREEN}✓${_NC} Shaders: ${SHADER_COUNT} files"
else
    echo -e "  ${_YELLOW}⚠${_NC} Shaders directory not found in build output (${SHADERS_DIR})"
fi

if [[ -d "${TEXTURES_DIR}" ]]; then
    TEXTURE_COUNT=$(find "${TEXTURES_DIR}" -type f | wc -l)
    echo -e "  ${_GREEN}✓${_NC} Textures: ${TEXTURE_COUNT} files"
else
    echo -e "  ${_YELLOW}⚠${_NC} Textures directory not found in build output (${TEXTURES_DIR})"
fi

# Final messages
EXECUTABLE="${BIN_DIR}/ArtGallery"
echo ""
if [[ -f "${EXECUTABLE}" ]]; then
    echo -e "${_GREEN}✓ Executable location:${_NC}"
    echo -e "  ${_GREEN}${EXECUTABLE}${_NC}"
    echo -e ""
    echo -e "To run:"
    echo -e "  ${_GREEN}${EXECUTABLE}${_NC}"
else
    echo -e "${_YELLOW}Warning: Executable not found at ${EXECUTABLE}${_NC}"
fi

echo ""
echo -e "${_BLUE}Build Summary:${_NC}"
echo -e "  Build Type:    ${_YELLOW}${BUILD_TYPE}${_NC}"
echo -e "  Compiler:      ${_YELLOW}${COMPILER}${_NC}"
echo -e "  Sanitizer:     ${_YELLOW}$([ "${ENABLE_SANITIZER}" -eq 1 ] && echo "ON" || echo "OFF")${_NC}"
echo -e "  Build Dir:     ${_YELLOW}${BUILD_DIR}${_NC}"
echo ""
echo -e "${_GREEN}Build finished.${_NC}"
