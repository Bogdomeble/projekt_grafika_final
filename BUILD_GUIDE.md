# Build Guide - Art Gallery 3D Renderer

This comprehensive guide covers building the Art Gallery 3D Renderer on all supported platforms.

## Table of Contents

1. [Linux (Ubuntu/Debian)](#linux-ubuntudebian)
2. [Linux (Fedora/RHEL)](#linux-fedorarhel)
3. [macOS](#macos)
4. [Windows (Visual Studio)](#windows-visual-studio)
5. [Windows (MinGW)](#windows-mingw)
6. [Windows (vcpkg)](#windows-vcpkg)
7. [Docker (Optional)](#docker-optional)
8. [Troubleshooting](#troubleshooting)

---

## Linux (Ubuntu/Debian)

### Prerequisites

```bash
sudo apt-get update
sudo apt-get install -y \
    cmake \
    build-essential \
    git \
    libglfw3-dev \
    libglm-dev \
    libgl1-mesa-dev \
    xorg-dev \
    libxrandr-dev \
    libxinerama-dev \
    libxcursor-dev \
    libxi-dev
```

### Build Instructions

```bash
# Navigate to project directory
cd projekt_grafika_final

# Create build directory
mkdir -p build && cd build

# Configure with CMake (Release build)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build with all available CPU cores
cmake --build . -j$(nproc)

# Alternative: Build with make
make -j$(nproc)

# Run the application
./bin/ArtGallery
```

### Debug Build (Linux)

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZER=ON ..
make -j$(nproc)
./bin/ArtGallery
```

### Optimization Flags

For maximum performance:

```bash
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_CXX_FLAGS="-O3 -march=native -flto" ..
make -j$(nproc)
```

---

## Linux (Fedora/RHEL)

### Prerequisites

```bash
sudo dnf install -y \
    cmake \
    gcc-c++ \
    git \
    glfw-devel \
    glm-devel \
    mesa-libGL-devel \
    libX11-devel \
    libXrandr-devel \
    libXinerama-devel \
    libXcursor-devel \
    libXi-devel
```

### Build Instructions

```bash
cd projekt_grafika_final
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
./bin/ArtGallery
```

---

## macOS

### Prerequisites (using Homebrew)

```bash
# Install Homebrew if not already installed
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install cmake glfw glm llvm
```

### Build Instructions

```bash
cd projekt_grafika_final
mkdir -p build && cd build

# Configure for macOS
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ ..

# Build
cmake --build . -j$(sysctl -n hw.ncpu)

# Run
./bin/ArtGallery
```

### Using Apple Silicon (M1/M2/M3)

```bash
cd projekt_grafika_final
mkdir -p build && cd build

cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_OSX_ARCHITECTURES=arm64 ..

cmake --build . -j$(sysctl -n hw.ncpu)
./bin/ArtGallery
```

### XCode Integration

```bash
cd projekt_grafika_final
mkdir -p build && cd build

# Generate XCode project
cmake -G Xcode -DCMAKE_BUILD_TYPE=Release ..

# Open in XCode
open ArtGalleryRenderer.xcodeproj

# Or build from command line
cmake --build . --config Release
```

---

## Windows (Visual Studio)

### Prerequisites

1. **Visual Studio 2019 or later** with C++ workload
2. **CMake 3.15+** (download from https://cmake.org/download/)
3. **Git** (download from https://git-scm.com/)

### Manual Library Installation

1. Download GLFW from https://www.glfw.org/download.html
2. Download GLM from https://github.com/g-truc/glm/releases
3. Extract to a known location (e.g., `C:\dev\libraries\`)

### Build Instructions (with manual libraries)

```bash
cd projekt_grafika_final
mkdir build
cd build

# Configure for Visual Studio
cmake -G "Visual Studio 17 2022" ^
       -DCMAKE_PREFIX_PATH="C:\dev\libraries\glfw;C:\dev\libraries\glm" ..

# Build
cmake --build . --config Release

# Run
.\bin\Release\ArtGallery.exe
```

### Build Instructions (with system libraries)

If libraries are installed to system paths:

```bash
cd projekt_grafika_final
mkdir build
cd build

cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
.\bin\Release\ArtGallery.exe
```

---

## Windows (MinGW)

### Prerequisites

1. **MinGW-w64** (download from https://www.mingw-w64.org/)
2. **CMake 3.15+**
3. **MSYS2** (recommended)

### Using MSYS2

```bash
# Install dependencies via pacman
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc mingw-w64-x86_64-glfw mingw-w64-x86_64-glm

# Build
cd projekt_grafika_final
mkdir build && cd build

cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make -j4

# Run
.\bin\ArtGallery.exe
```

### Using CMake with MinGW

```bash
cd projekt_grafika_final
mkdir build && cd build

cmake -G "MinGW Makefiles" \
       -DCMAKE_CXX_COMPILER=g++ \
       -DCMAKE_C_COMPILER=gcc \
       -DCMAKE_BUILD_TYPE=Release ..

mingw32-make -j4
.\bin\ArtGallery.exe
```

---

## Windows (vcpkg)

### vcpkg Setup

```bash
# Clone vcpkg repository
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg

# Run bootstrap script
.\bootstrap-vcpkg.bat

# Integrate with Visual Studio (optional)
.\vcpkg integrate install
```

### Install Dependencies

```bash
# For x64 Release
.\vcpkg install glfw3:x64-windows glm:x64-windows

# For ARM64 (if needed)
.\vcpkg install glfw3:arm64-windows glm:arm64-windows
```

### Build with vcpkg

```bash
cd projekt_grafika_final
mkdir build && cd build

cmake -G "Visual Studio 17 2022" \
       -DCMAKE_TOOLCHAIN_FILE=<vcpkg-root>/scripts/buildsystems/vcpkg.cmake \
       -DVCPKG_TARGET_TRIPLET=x64-windows ..

cmake --build . --config Release
.\bin\Release\ArtGallery.exe
```

Replace `<vcpkg-root>` with your vcpkg installation path.

---

## Docker (Optional)

### Building with Docker

Create a `Dockerfile` in the project root:

```dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    git \
    libglfw3-dev \
    libglm-dev \
    libgl1-mesa-dev \
    xorg-dev

WORKDIR /app
COPY . /app/

RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . -j$(nproc)

ENTRYPOINT ["./build/bin/ArtGallery"]
```

### Building Docker Image

```bash
docker build -t art-gallery:latest .
```

### Running with X11 Forwarding (Linux)

```bash
docker run -it \
    --network host \
    -e DISPLAY=$DISPLAY \
    -v /tmp/.X11-unix:/tmp/.X11-unix \
    art-gallery:latest
```

---

## CMake Configuration Options

### Available Options

```bash
# Build type
-DCMAKE_BUILD_TYPE=Release|Debug|RelWithDebInfo|MinSizeRel

# Enable AddressSanitizer (Debug only, Linux/macOS)
-DENABLE_SANITIZER=ON|OFF

# Enable verbose logging
-DVERBOSE_LOGGING=ON|OFF

# Custom compiler
-DCMAKE_CXX_COMPILER=/path/to/compiler

# Custom install prefix
-DCMAKE_INSTALL_PREFIX=/usr/local
```

### Example: Full Debug Build with Sanitizer

```bash
cmake -DCMAKE_BUILD_TYPE=Debug \
       -DENABLE_SANITIZER=ON \
       -DVERBOSE_LOGGING=ON \
       -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
```

---

## Installation

### Install to System Directories

```bash
cd build
sudo cmake --install . --prefix /usr/local
```

The executable will be installed to `/usr/local/bin/ArtGallery` and assets to `/usr/local/share/art-gallery/`.

### Create Custom Install

```bash
cd build
cmake --install . --prefix ~/.local
export PATH="$PATH:~/.local/bin"
```

---

## Troubleshooting

### "GLFW not found"

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install libglfw3-dev

# macOS
brew install glfw

# Verify installation
pkg-config --cflags --libs glfw3
```

### "GLM not found"

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install libglm-dev

# macOS
brew install glm

# Windows (vcpkg)
vcpkg install glm:x64-windows
```

### "OpenGL not found"

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install libgl1-mesa-dev libglu1-mesa-dev

# macOS - Usually pre-installed

# Windows - Usually comes with Visual Studio
```

### CMake Cache Issues

Clear CMake cache and rebuild:

```bash
cd build
rm -rf CMakeCache.txt CMakeFiles/
cmake -DCMAKE_BUILD_TYPE=Release ..
make clean
make -j$(nproc)
```

### Shader Loading Errors

Ensure assets are copied:

```bash
# Check if assets exist in build directory
ls -la build/bin/shaders/
ls -la build/bin/textures/

# If not, manually copy:
cp -r assets/* build/bin/
```

### Low Performance

Enable optimizations:

```bash
cd build
rm -rf CMakeCache.txt CMakeFiles/
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native" ..
make -j$(nproc)
```

### Compilation Errors with Old Compiler

Update your compiler:

**Ubuntu/Debian:**
```bash
sudo apt-get install g++-11  # or higher
cmake -DCMAKE_CXX_COMPILER=g++-11 ..
```

**macOS:**
```bash
brew install gcc@12
cmake -DCMAKE_CXX_COMPILER=g++-12 ..
```

### Runtime Crashes

**Check OpenGL support:**
```bash
glxinfo | grep "OpenGL version"  # Linux
glxgears                           # Linux
glfwGetVersionString()             # In code
```

**Enable debug symbols:**
```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j$(nproc)
gdb ./bin/ArtGallery
```

---

## Performance Tuning

### For Maximum Speed

```bash
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_CXX_FLAGS="-O3 -march=native -ffast-math -flto" \
       -DCMAKE_EXE_LINKER_FLAGS="-flto" ..
make -j$(nproc)
```

### For Minimum Size

```bash
cmake -DCMAKE_BUILD_TYPE=MinSizeRel ..
make -j$(nproc)
strip bin/ArtGallery
```

### Profile the Build

```bash
cmake --build . --verbose
time make -j1  # Single-threaded for accurate timing
```

---

## Clean Build

Complete rebuild from scratch:

```bash
cd projekt_grafika_final
rm -rf build
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

---

## Next Steps

1. Run the application: `./bin/ArtGallery`
2. See [README.md](README.md) for usage instructions
3. Check [docs.md](docs.md) for technical documentation
4. Modify the project as needed for your use case

---

**Last Updated**: 2025