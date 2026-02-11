#!/bin/bash

# Art Gallery 3D Renderer - Build Script
# Supports Linux, macOS, and other Unix-like systems
# Usage: ./build.sh [options]

set -e  # Exit on error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Default values
BUILD_TYPE="Release"
BUILD_DIR="build"
ENABLE_SANITIZER=0
VERBOSE=0
JOBS=$(nproc 2>/dev/null || echo 4)
SHOW_HELP=0

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        -s|--sanitizer)
            ENABLE_SANITIZER=1
            shift
            ;;
        -v|--verbose)
            VERBOSE=1
            shift
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        -b|--build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        -c|--clean)
            echo -e "${BLUE}Cleaning build directory...${NC}"
            rm -rf "$BUILD_DIR"
            echo -e "${GREEN}Build directory cleaned.${NC}"
            exit 0
            ;;
        -h|--help)
            SHOW_HELP=1
            shift
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            SHOW_HELP=1
            shift
            ;;
    esac
done

# Show help
if [ $SHOW_HELP -eq 1 ]; then
    echo "Art Gallery 3D Renderer - Build Script"
    echo ""
    echo "Usage: ./build.sh [options]"
    echo ""
    echo "Options:"
    echo "  -d, --debug              Build with debug symbols (default: Release)"
    echo "  -r, --release            Build optimized Release build (default)"
    echo "  -s, --sanitizer          Enable AddressSanitizer (Debug only)"
    echo "  -v, --verbose            Show verbose build output"
    echo "  -j, --jobs NUM           Number of parallel jobs (default: auto-detect)"
    echo "  -b, --build-dir DIR      Build directory (default: build)"
    echo "  -c, --clean              Clean build directory and exit"
    echo "  -h, --help               Show this help message"
    echo ""
    echo "Examples:"
    echo "  ./build.sh                    # Build Release version"
    echo "  ./build.sh --debug            # Build Debug version"
    echo "  ./build.sh -d -s              # Debug with AddressSanitizer"
    echo "  ./build.sh -j 8               # Use 8 parallel jobs"
    echo ""
    exit 0
fi

# Print configuration
echo -e "${BLUE}================================${NC}"
echo -e "${BLUE}Art Gallery 3D Renderer${NC}"
echo -e "${BLUE}================================${NC}"
echo -e "Build Type:        ${YELLOW}${BUILD_TYPE}${NC}"
echo -e "Build Directory:   ${YELLOW}${BUILD_DIR}${NC}"
echo -e "Parallel Jobs:     ${YELLOW}${JOBS}${NC}"
echo -e "Sanitizer:         ${YELLOW}$([ $ENABLE_SANITIZER -eq 1 ] && echo "ON" || echo "OFF")${NC}"
echo -e "Verbose:           ${YELLOW}$([ $VERBOSE -eq 1 ] && echo "ON" || echo "OFF")${NC}"
echo -e "${BLUE}================================${NC}"
echo ""

# Check if CMake is available
if ! command -v cmake &> /dev/null; then
    echo -e "${RED}Error: CMake is not installed.${NC}"
    echo "Please install CMake 3.15 or later:"
    echo "  Ubuntu/Debian: sudo apt-get install cmake"
    echo "  macOS:         brew install cmake"
    exit 1
fi

echo -e "${GREEN}✓${NC} CMake found: $(cmake --version | head -n1)"

# Check C++ compiler
if ! command -v g++ &> /dev/null && ! command -v clang++ &> /dev/null; then
    echo -e "${RED}Error: No C++ compiler found.${NC}"
    echo "Please install GCC or Clang:"
    echo "  Ubuntu/Debian: sudo apt-get install build-essential"
    echo "  macOS:         brew install gcc"
    exit 1
fi

if command -v g++ &> /dev/null; then
    COMPILER_INFO="GCC $(g++ --version | head -n1 | grep -oP '\d+\.\d+')"
elif command -v clang++ &> /dev/null; then
    COMPILER_INFO="Clang $(clang++ --version | head -n1 | grep -oP '\d+\.\d+')"
fi
echo -e "${GREEN}✓${NC} Compiler found: $COMPILER_INFO"

# Check required libraries
echo ""
echo -e "${BLUE}Checking dependencies...${NC}"

# Check GLFW
if pkg-config --exists glfw3 2>/dev/null; then
    echo -e "${GREEN}✓${NC} GLFW3 found"
else
    echo -e "${YELLOW}⚠${NC} GLFW3 not found (may fail during build)"
    echo "  Install: sudo apt-get install libglfw3-dev (Ubuntu/Debian)"
    echo "  Install: brew install glfw (macOS)"
fi

# Check GLM
if pkg-config --exists glm 2>/dev/null; then
    echo -e "${GREEN}✓${NC} GLM found"
else
    echo -e "${YELLOW}⚠${NC} GLM not found (may fail during build)"
    echo "  Install: sudo apt-get install libglm-dev (Ubuntu/Debian)"
    echo "  Install: brew install glm (macOS)"
fi

# Check OpenGL
if pkg-config --exists gl 2>/dev/null; then
    echo -e "${GREEN}✓${NC} OpenGL found"
else
    echo -e "${YELLOW}⚠${NC} OpenGL development files not found"
    echo "  Install: sudo apt-get install libgl1-mesa-dev (Ubuntu/Debian)"
fi

echo ""

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${BLUE}Creating build directory: ${BUILD_DIR}${NC}"
    mkdir -p "$BUILD_DIR"
fi

cd "$BUILD_DIR"

# Configure CMake
echo -e "${BLUE}Configuring CMake...${NC}"
CMAKE_FLAGS="-DCMAKE_BUILD_TYPE=$BUILD_TYPE"

if [ $ENABLE_SANITIZER -eq 1 ] && [ "$BUILD_TYPE" = "Debug" ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DENABLE_SANITIZER=ON"
    echo -e "${YELLOW}Enabling AddressSanitizer${NC}"
fi

if [ $VERBOSE -eq 1 ]; then
    CMAKE_FLAGS="$CMAKE_FLAGS -DCMAKE_VERBOSE_MAKEFILE=ON"
    CMAKE_CALL="cmake $CMAKE_FLAGS .."
else
    CMAKE_CALL="cmake $CMAKE_FLAGS .."
fi

if ! eval "$CMAKE_CALL"; then
    echo -e "${RED}CMake configuration failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ CMake configuration complete${NC}"
echo ""

# Build
echo -e "${BLUE}Building project (${JOBS} jobs)...${NC}"

if [ $VERBOSE -eq 1 ]; then
    if ! cmake --build . --config "$BUILD_TYPE" -j "$JOBS" --verbose; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
else
    if ! cmake --build . --config "$BUILD_TYPE" -j "$JOBS"; then
        echo -e "${RED}Build failed!${NC}"
        exit 1
    fi
fi

echo ""
echo -e "${GREEN}✓ Build complete!${NC}"
echo ""

# Print executable location
EXECUTABLE="./bin/ArtGallery"
if [ -f "$EXECUTABLE" ]; then
    echo -e "${BLUE}Executable location:${NC}"
    echo -e "  ${GREEN}${EXECUTABLE}${NC}"
    echo ""
    echo -e "${BLUE}To run the application:${NC}"
    echo -e "  ${GREEN}${EXECUTABLE}${NC}"
    echo ""
else
    echo -e "${YELLOW}Warning: Executable not found at ${EXECUTABLE}${NC}"
fi

# Print build summary
echo -e "${BLUE}Build Summary:${NC}"
echo -e "  Build Type:    ${YELLOW}${BUILD_TYPE}${NC}"
echo -e "  Compiler:      ${YELLOW}${COMPILER_INFO}${NC}"
echo -e "  Sanitizer:     ${YELLOW}$([ $ENABLE_SANITIZER -eq 1 ] && echo "ON" || echo "OFF")${NC}"
echo -e "  Build Dir:     ${YELLOW}${BUILD_DIR}${NC}"
echo ""

# Check for assets
echo -e "${BLUE}Asset Status:${NC}"
if [ -d "bin/shaders" ]; then
    SHADER_COUNT=$(find bin/shaders -type f | wc -l)
    echo -e "  ${GREEN}✓${NC} Shaders: $SHADER_COUNT files"
else
    echo -e "  ${YELLOW}⚠${NC} Shaders directory not found"
fi

if [ -d "bin/textures" ]; then
    TEXTURE_COUNT=$(find bin/textures -type f | wc -l)
    echo -e "  ${GREEN}✓${NC} Textures: $TEXTURE_COUNT files"
else
    echo -e "  ${YELLOW}⚠${NC} Textures directory not found"
fi

echo ""
echo -e "${GREEN}Build completed successfully!${NC}"
echo ""
