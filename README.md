# Art Gallery 3D Renderer

A modern OpenGL-based 3D scene renderer featuring an art gallery with realistic lighting, texturing, and various geometric shapes.

## Overview

This project demonstrates advanced computer graphics concepts using:
- **OpenGL 3.3+** - Modern graphics rendering
- **GLFW 3.3** - Window management and input handling
- **GLM** - Mathematics library for transformations
- **GLAD** - OpenGL function loader
- **STB Image** - Texture loading

## Project Structure

```
projekt_grafika_final/
├── CMakeLists.txt              # Modern CMake build configuration
├── README.md                   # This file
├── docs.md                     # Detailed technical documentation
├── .gitignore                  # Git ignore patterns
│
├── src/                        # Source files (.cpp)
│   ├── main.cpp               # Application entry point
│   ├── shaderClass.cpp        # Shader compilation and management
│   ├── texture.cpp            # Texture loading and management
│   ├── camera.cpp             # Camera system implementation
│   ├── shape.cpp              # Base shape class
│   ├── cube.cpp               # Cube geometry
│   ├── pyramid.cpp            # Pyramid geometry
│   ├── plane.cpp              # Plane geometry
│   ├── sphere.cpp             # Sphere geometry
│   ├── cylinder.cpp           # Cylinder geometry
│   ├── TrapezoidPrism.cpp     # Trapezoid prism geometry
│   ├── VAO.cpp                # Vertex Array Object management
│   ├── VBO.cpp                # Vertex Buffer Object management
│   ├── EBO.cpp                # Element Buffer Object management
│   ├── glad.c                 # OpenGL function pointers
│   ├── stb.cpp                # STB image library implementation
│   └── include.cpp            # Utility implementations
│
├── include/                    # Header files (.h)
│   ├── include.h              # Common includes
│   ├── shaderClass.h
│   ├── texture.h
│   ├── camera.h
│   ├── light.h                # Lighting structures
│   ├── shape.h
│   ├── cube.h
│   ├── pyramid.h
│   ├── plane.h
│   ├── sphere.h
│   ├── cylinder.h
│   ├── TrapezoidPrism.h
│   ├── VAO.h
│   ├── VBO.h
│   └── EBO.h
│
└── assets/                     # Resource files
    ├── shaders/                # GLSL shader files
    │   ├── default.vert       # Default vertex shader
    │   ├── default.frag       # Default fragment shader
    │   ├── light.vert         # Lighting vertex shader
    │   └── light.frag         # Lighting fragment shader
    └── textures/               # Texture images
        ├── art1.png to art11.png      # Artwork textures
        ├── brick*.png                 # Brick wall textures
        ├── wood_texture_*.png         # Wood textures
        ├── metal*.png / metal*.jpg    # Metal textures
        ├── floor*.jpg / floor*.png    # Floor textures
        ├── marble.jpg                 # Marble texture
        ├── red.jpg                    # Red material
        ├── wall_plaster.png           # Wall texture
        └── world.png                  # Cubemap texture
```

## Prerequisites

### System Requirements
- **C++ 17** or later compiler
- **CMake 3.15+**
- **OpenGL 3.3+** compatible graphics card

### Dependencies

#### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install -y \
    cmake \
    libglfw3-dev \
    libglm-dev \
    libgl1-mesa-dev \
    xorg-dev
```

#### macOS (using Homebrew)
```bash
brew install cmake glfw glm
```

#### Windows
- Install **CMake** from https://cmake.org/download/
- Install **Visual Studio 2019+** or **MinGW**
- Use **vcpkg** for dependencies:
  ```bash
  vcpkg install glfw3:x64-windows glm:x64-windows
  ```

## Building

### Quick Start (Linux/macOS)

```bash
cd projekt_grafika_final
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
./bin/ArtGallery
```

### Windows (Visual Studio)

```bash
cd projekt_grafika_final
mkdir build
cd build
cmake -G "Visual Studio 16 2019" ..
cmake --build . --config Release
.\bin\Release\ArtGallery.exe
```

### Windows (MinGW)

```bash
cd projekt_grafika_final
mkdir build
cd build
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..
mingw32-make -j4
.\bin\ArtGallery.exe
```

### Build Options

```bash
# Debug build with symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Release build with optimizations
cmake -DCMAKE_BUILD_TYPE=Release ..

# With sanitizers (Linux/macOS)
cmake -DENABLE_SANITIZER=ON ..

# With verbose logging
cmake -DVERBOSE_LOGGING=ON ..
```

## Usage

### Running the Application

After building, run the executable:

```bash
# Linux/macOS
./bin/ArtGallery

# Windows
bin\ArtGallery.exe
```

### Camera Controls

- **Mouse Movement** - Look around the scene
- **W/A/S/D** - Move forward/left/backward/right
- **Space** - Move up
- **Shift** - Move down
- **ESC** - Exit application

### Display Resolution

Default resolution: **1920×1080**

Edit in `src/main.cpp`:
```cpp
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
```

## Architecture

### Core Components

1. **Shader System** (`shaderClass`)
   - Compiles GLSL shaders
   - Manages uniform variables
   - Supports vertex and fragment shaders

2. **Texture Management** (`texture`)
   - Loads PNG and JPG textures
   - Handles texture binding and sampling
   - Supports multiple texture units

3. **Camera System** (`camera`)
   - Perspective projection
   - Free camera movement
   - View matrix management

4. **Geometry Classes**
   - Base `Shape` class with common interface
   - Specialized classes: `Cube`, `Pyramid`, `Plane`, `Sphere`, `Cylinder`, `TrapezoidPrism`
   - Vertex/normal/texture coordinate generation

5. **Buffer Management**
   - **VAO** - Vertex Array Object (stores vertex configuration)
   - **VBO** - Vertex Buffer Object (stores vertex data)
   - **EBO** - Element Buffer Object (stores indices)

6. **Lighting** (`light`)
   - Multiple light sources support
   - Phong/Blinn-Phong shading
   - Material properties

## Performance Optimization

The project includes several optimizations:

- **Link-Time Optimization (LTO)** - Enabled in Release builds
- **Native Architecture Optimization** - Compiled for your CPU (`-march=native`)
- **Fast Floating-Point Model** - MSVC: `/fp:fast`
- **Compiler Vectorization** - Automatic SIMD optimizations
- **Asset Preloading** - All assets loaded before rendering loop

## Known Issues & Future Improvements

Refer to `docs.md` for detailed documentation on:
- Current limitations
- Planned enhancements
- Technical depth on rendering pipeline

## Development Guidelines

### Adding New Shapes

1. Create header in `include/newshape.h`
2. Implement in `src/newshape.cpp`
3. Inherit from `Shape` base class
4. Implement `generate()` method for vertex data
5. Add to CMakeLists.txt source list

### Adding New Shaders

1. Create `.vert` and `.frag` files in `assets/shaders/`
2. Load using `Shader shader("path/to/shader");`
3. Set uniforms: `shader.use(); shader.setMat4("matrix", value);`

### Modifying Renderer

The main rendering loop is in `src/main.cpp`:
- Modify `main()` function to change scene setup
- Adjust lighting by modifying light parameters
- Change object positions/rotations/scales

## Troubleshooting

### Missing Dependencies
```bash
# Install missing packages (Ubuntu)
sudo apt-get install <package-name>

# Update CMake lists if paths changed
rm -rf build && mkdir build && cd build && cmake ..
```

### Graphics Issues
- Ensure OpenGL 3.3+ support: Check GPU drivers
- Update graphics drivers for best compatibility
- Test with software renderer if GPU fails

### Build Issues
- Clean build: `rm -rf build && mkdir build`
- Check CMake version: `cmake --version` (need 3.15+)
- Verify all dependencies installed

## Contributing

For improvements or bug reports:
1. Create a new branch for your feature
2. Follow existing code style
3. Update documentation accordingly
4. Test on at least one platform

## License

This project is provided as-is for educational purposes.

## References

- [LearnOpenGL](https://learnopengl.com/) - OpenGL tutorials
- [GLM Documentation](https://github.com/g-truc/glm/wiki)
- [GLFW Documentation](https://www.glfw.org/documentation.html)
- [Official OpenGL Registry](https://www.khronos.org/opengl/)

## Authors

- **Original Authors**: STM32F439 and contributors
- **Last Updated**: 2025

---

**For detailed technical documentation, see `docs.md`**