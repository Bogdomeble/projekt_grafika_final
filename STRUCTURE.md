# Project Structure Documentation

## Overview

The Art Gallery 3D Renderer project has been reorganized into a modern, maintainable structure following CMake best practices.

## Directory Tree

```
projekt_grafika_final/
│
├── CMakeLists.txt                 # Modern CMake build configuration
├── README.md                       # Quick start guide and overview
├── BUILD_GUIDE.md                 # Comprehensive platform-specific build instructions
├── STRUCTURE.md                   # This file - project structure documentation
├── docs.md                        # Detailed technical documentation
├── .gitignore                     # Git ignore patterns
│
├── src/                           # Source code files (.cpp)
│   ├── main.cpp                   # Application entry point and main loop
│   ├── shaderClass.cpp            # Shader compilation and uniform management
│   ├── texture.cpp                # Texture loading and GPU binding
│   ├── camera.cpp                 # Camera system with view matrix calculations
│   ├── shape.cpp                  # Base Shape class implementation
│   ├── cube.cpp                   # Cube geometry generation
│   ├── pyramid.cpp                # Pyramid geometry generation
│   ├── plane.cpp                  # Plane/quad geometry generation
│   ├── sphere.cpp                 # Sphere geometry generation (subdivisions)
│   ├── cylinder.cpp               # Cylinder geometry generation
│   ├── TrapezoidPrism.cpp         # Custom trapezoid prism geometry
│   ├── VAO.cpp                    # Vertex Array Object management
│   ├── VBO.cpp                    # Vertex Buffer Object management
│   ├── EBO.cpp                    # Element Buffer Object management
│   ├── glad.c                     # OpenGL function pointers and loader
│   ├── stb.cpp                    # STB image library implementation wrapper
│   └── include.cpp                # Utility function implementations
│
├── include/                       # Header files (.h)
│   ├── include.h                  # Common includes and forward declarations
│   ├── shaderClass.h              # Shader class definition
│   ├── texture.h                  # Texture class definition
│   ├── camera.h                   # Camera class definition
│   ├── light.h                    # Lighting structures and definitions
│   ├── shape.h                    # Base Shape class definition
│   ├── cube.h                     # Cube class definition
│   ├── pyramid.h                  # Pyramid class definition
│   ├── plane.h                    # Plane class definition
│   ├── sphere.h                   # Sphere class definition
│   ├── cylinder.h                 # Cylinder class definition
│   ├── TrapezoidPrism.h           # TrapezoidPrism class definition
│   ├── VAO.h                      # VAO class definition
│   ├── VBO.h                      # VBO class definition
│   └── EBO.h                      # EBO class definition
│
└── assets/                        # Game assets and resources
    ├── shaders/                   # GLSL shader files
    │   ├── default.vert           # Default vertex shader
    │   ├── default.frag           # Default fragment shader
    │   ├── light.vert             # Lighting vertex shader
    │   └── light.frag             # Lighting fragment shader
    │
    └── textures/                  # Texture image files
        ├── Artwork Textures
        │   ├── art1.png
        │   ├── art2.png
        │   ├── art3.png
        │   ├── art4.png
        │   ├── art5.png
        │   ├── art6.png
        │   ├── art7.png
        │   ├── art8.png
        │   ├── art9.png
        │   ├── art10.png
        │   └── art11.png
        │
        ├── Building Materials
        │   ├── brick.png
        │   ├── brick3.png
        │   ├── brick4.png
        │   ├── wall_plaster.png
        │   └── marble.jpg
        │
        ├── Wood Textures
        │   ├── wood_texture_horizontal.png
        │   ├── wood_texture_vertical.png
        │   └── floor_wood.png
        │
        ├── Metal Textures
        │   ├── metal.png
        │   ├── metal_texture.png
        │   └── metal3.jpg
        │
        ├── Floor Textures
        │   ├── floor.jpg
        │   └── floor_wood.png
        │
        ├── Special Textures
        │   ├── red.jpg
        │   └── world.png
        │
        └── (auto-copied to build/textures/ after compilation)

## Build Output Structure

After building, the `build/` directory is created with the following structure:

```
build/
├── bin/                           # Executable directory
│   └── ArtGallery                 # Main executable
│   ├── shaders/                   # Copied shader files
│   │   ├── default.vert
│   │   ├── default.frag
│   │   ├── light.vert
│   │   └── light.frag
│   └── textures/                  # Copied texture files
│       └── (all textures from assets/textures/)
│
├── lib/                           # Static/shared libraries
├── CMakeCache.txt
├── CMakeFiles/
├── Makefile (or Visual Studio files)
└── cmake_install.cmake
```

## File Organization Principles

### Source Files (src/)
- **Logical grouping**: Core functionality (main, shader, texture, camera)
- **Geometry implementations**: Separate files for each shape type
- **Buffer management**: VAO/VBO/EBO in dedicated files
- **Third-party code**: glad.c and stb.cpp isolated for easy updates

### Header Files (include/)
- **One-to-one mapping**: Each .cpp typically has a corresponding .h
- **Include guards**: All headers protected with `#ifndef`
- **Forward declarations**: Minimize unnecessary includes
- **Clear dependencies**: Public APIs well-defined

### Assets (assets/)
- **Shaders**: Separated by type (vertex, fragment)
- **Textures**: Organized by material/purpose
- **Runtime copying**: CMake automatically copies to build directory
- **No hard-coded paths**: Asset paths defined in CMakeLists.txt

## Compilation Flow

```
CMakeLists.txt
    ↓
[Dependency detection: GLFW, GLM, OpenGL, glad, stb]
    ↓
[Source file compilation]
    ↓
src/*.cpp → object files (.o/.obj)
    ↓
[Linking]
    ↓
bin/ArtGallery executable
    ↓
[Post-build commands]
    ↓
Copy shaders → bin/shaders/
Copy textures → bin/textures/
    ↓
Ready to run!
```

## Dependency Graph

```
main.cpp
    ├── shaderClass.h         (Shader management)
    ├── texture.h             (Texture loading)
    ├── camera.h              (View matrix)
    ├── shape.h               (Base geometry)
    ├── cube.h, pyramid.h, etc. (Shape implementations)
    ├── light.h               (Lighting structures)
    ├── VAO.h, VBO.h, EBO.h   (GPU buffers)
    └── glad/glfw/glm         (External libraries)

External Libraries:
    ├── GLFW 3.3+             (Window/Input)
    ├── GLAD                  (OpenGL loader)
    ├── GLM                   (Math library)
    └── stb_image             (Image loading)
```

## Naming Conventions

### C++ Classes
- **Shape classes**: Capitalized (Cube, Pyramid, Sphere)
- **Manager classes**: Capitalized with noun (Shader, Texture, Camera)
- **Buffer classes**: Abbreviated caps (VAO, VBO, EBO)

### File Names
- **Headers**: `.h` extension
- **Source**: `.cpp` extension
- **Shaders**: `.vert` (vertex), `.frag` (fragment)
- **Textures**: `.png` or `.jpg`

### Variables & Functions
- **member variables**: Typically `camelCase`
- **functions**: `camelCase`
- **constants**: `UPPER_SNAKE_CASE`
- **macros**: `UPPER_SNAKE_CASE`

## Key Features of New Structure

### 1. **Clean Separation**
- Source and headers in separate directories
- Assets organized by type
- Build artifacts isolated from source

### 2. **CMake Integration**
- Modern CMake practices (3.15+)
- Automatic asset copying
- Cross-platform support (Windows, macOS, Linux)
- Compiler optimizations for Release builds

### 3. **Scalability**
- Easy to add new shapes: create `newshape.h/cpp`, update CMakeLists.txt
- Easy to add new shaders: place in `assets/shaders/`, reference in code
- Easy to add new textures: place in `assets/textures/`, auto-copied

### 4. **Documentation**
- README.md: Quick start
- BUILD_GUIDE.md: Platform-specific building
- docs.md: Technical details
- STRUCTURE.md: This file
- CMakeLists.txt: Build configuration comments

### 5. **Version Control**
- .gitignore: Prevents build artifacts from being committed
- Only source and assets tracked
- Build directory completely ignored

## Migration from Old Structure

### What Changed
```
Before:
projekt_grafika_final/
└── Projekt_grafika_final/         (Everything mixed together)
    ├── *.cpp
    ├── *.h
    ├── *.png
    ├── *.jpg
    ├── *.vert
    ├── *.frag
    └── x64/                        (Build artifacts)

After:
projekt_grafika_final/
├── src/                           (Only .cpp files)
├── include/                       (Only .h files)
├── assets/
│   ├── shaders/                  (Shader files)
│   └── textures/                 (Texture files)
└── build/                         (Automatically generated)
```

### What Stayed the Same
- All source code functionality preserved
- All assets intact and organized
- Same rendering behavior
- Same visual output

## Performance Optimizations

The new structure enables several optimizations:

1. **Incremental compilation**: Only changed files recompiled
2. **Link-time optimization (LTO)**: Enabled in Release builds
3. **Native architecture optimization**: `-march=native` flag
4. **Parallel building**: CMake supports `-j` for multi-core compilation
5. **ccache integration**: Compiler cache speeds up rebuilds

## Building & Running

### Quick Build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(nproc)
./bin/ArtGallery
```

### Files Generated During Build
- Object files: `build/CMakeFiles/`
- Executable: `build/bin/ArtGallery`
- Shaders: `build/bin/shaders/`
- Textures: `build/bin/textures/`

## Maintenance

### Adding a New Geometry Class
1. Create `include/newshape.h` (class definition)
2. Create `src/newshape.cpp` (implementation)
3. Add to `CMakeLists.txt` in `SOURCE_FILES`
4. Include header in `include/include.h`

### Adding a New Shader
1. Create `.vert` or `.frag` file in `assets/shaders/`
2. CMake automatically copies it during build
3. Load in code: `Shader shader("shaders/myshader");`

### Adding a New Texture
1. Place image file in `assets/textures/`
2. CMake automatically copies it during build
3. Load in code: `Texture texture("textures/mytexture.png");`

## Best Practices

1. **Always build in a separate directory**: `mkdir build && cd build`
2. **Use Release builds for production**: `-DCMAKE_BUILD_TYPE=Release`
3. **Keep assets in version control**: Important for project reproducibility
4. **Don't modify CMakeLists.txt manually**: Use CMake variables
5. **Document new classes**: Add comments to headers
6. **Test on multiple platforms**: Windows, macOS, Linux if possible

## Troubleshooting File Issues

### "File not found" errors
- Check paths in CMakeLists.txt
- Ensure files are in correct directories
- Run from build directory when executing

### Shader loading fails
- Verify shaders are in `build/bin/shaders/`
- Check CMake post-build commands executed
- Manually copy if needed: `cp -r assets/shaders build/bin/`

### Texture loading fails
- Verify textures are in `build/bin/textures/`
- Check file names in code match actual files
- Ensure image format is supported (PNG, JPG)

---

**Last Updated**: 2025
**Structure Version**: 2.0 (Modern CMake refactoring)