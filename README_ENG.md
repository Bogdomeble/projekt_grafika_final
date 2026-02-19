Project: OpenGL 3D Scene Renderer (Art Gallery)

Summary
A compact C++ OpenGL application that renders a simple 3D scene (art gallery). Demonstrates:
- Mesh abstraction (Shape subclasses: Cube, Plane, Pyramid, Sphere, Cylinder)
- Shader-based lighting (vertex + fragment shaders)
- Texturing via stb_image
- Camera (FPS-style movement and mouse look)

How it works (brief)
1. Initialization
   - Initialize GLFW window and input callbacks.
   - Load OpenGL function pointers (GLAD).
   - Create shader programs from GLSL files.
   - Load textures with stb_image.

2. Scene setup
   - Instantiate Shape-derived objects, set model transforms and textures.
   - Call `setupMesh()` to create VAO/VBO/EBO for each shape.
   - Configure lights and camera.

3. Render loop
   - Compute delta time and update camera inputs.
   - Clear buffers and set per-frame uniforms (camera matrix, light properties).
   - For each object: bind shader, set model matrix, bind texture, draw with `glDrawElements`.
   - Swap buffers and poll events.

Project layout (short)
- `src/`      : implementation files (`main.cpp`, shape implementations, shader, texture, camera, VAO/VBO/EBO)
- `include/`  : headers
- `assets/`   : `shaders/*.vert|.frag`, `textures/*.png|.jpg`
- `CMakeLists.txt` or direct `g++` build

Build and run (recommended)
Prerequisites: development headers/libraries for GLFW, OpenGL, and a C++17 compiler; GLM and stb_image available in include path.

CMake (recommended)
1. mkdir build && cd build
2. cmake ..
3. cmake --build . --config Release
4. Copy `assets/` next to the executable or ensure runtime paths are correct.
5. Run: `./gallery` (or appropriate executable name on Windows)

Minimal g++ example (Linux)
g++ src/*.cpp -Iinclude -o gallery -lglfw -lGL -ldl -pthread
Ensure `glad.c` is compiled or GL loader is linked and asset paths are correct.

Runtime notes
- Shaders and textures must be accessible at the paths used in code (common path: `assets/shaders/` and `assets/textures/`).
- Window title may be updated with FPS counter if enabled in `main.cpp`.

Known issues (concise)
- Asset path errors: program will fail if shader/texture files are missing or paths are incorrect.
- Case-sensitivity: file includes and asset paths must match the OS filesystem (Linux is case-sensitive).
- Normal transformation: if non-uniform model scaling is used, normals must be transformed with the normal matrix (transpose(inverse(model))) to get correct lighting.
- Camera sensitivity / deltaTime: incorrect initialization or hardcoded values can cause jumpy or overly sensitive camera movement. Use dynamic deltaTime and clamp extreme values.
- Error handling: some helper functions may exit on failure; check console logs for shader compile/link errors.
- Platform differences: Windows-specific includes must be removed for cross-platform builds.

How to debug common failures
- Check console for shader compile/link logs.
- Verify asset paths relative to executable.
- Run under a debugger or add logging around file loads and OpenGL errors (`glGetError`).
- Validate VBO/EBO sizes and index counts if geometry does not render.

Contact / contribution
- Update `assets/` and headers to fix path or case issues.
- Keep changes minimal and document any path or dependency assumptions.
