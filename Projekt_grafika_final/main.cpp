// Try to include all necessary headers here, or in a common "include.h"
#include <iostream>
#include <vector>
#include <memory>
#include <string> // For std::string in case any class uses it

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Your custom classes
#include "shaderClass.h" // Assuming this is where Shader class is defined
#include "texture.h"     // Assuming this is where Texture class is defined
#include "camera.h"      // Assuming this is where Camera class is defined
#include "VAO.h"         // If not included via Shape.h
#include "VBO.h"         // If not included via Shape.h
#include "EBO.h"         // If not included via Shape.h
#include "Shape.h"
#include "Cube.h"
#include "Pyramid.h"
#include "Plane.h"
#include "Sphere.h"
#include "Cylinder.h"   

// Constants
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_STENCIL_BITS, 8); // Ensure stencil buffer is allocated

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL Advanced Scene", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    // glfwSwapInterval(1); // Optional: Enable V-Sync

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate(); // Terminate GLFW if GLAD fails
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Global OpenGL state
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // --- Shaders ---
    Shader objectShader("default.vert", "default.frag");
    Shader lightShader("light.vert", "light.frag");

    // --- Camera ---
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 2.5f, 8.0f)); // Adjusted camera start

    // --- Textures ---
    // Ensure your Texture class sets ID to 0 if stbi_load fails.

    Texture woodTexture("wood.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture brickTexture("brick3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE); // Slot can be same, it's about current bind
    Texture metalTexture("image.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    // Critical: Check if textures loaded successfully
    if (woodTexture.ID == 0) { std::cerr << "ERROR::TEXTURE: woodTexture failed to load. Check file path and integrity." << std::endl; }
    if (brickTexture.ID == 0) { std::cerr << "ERROR::TEXTURE: brickTexture failed to load. Check file path and integrity." << std::endl; }
    if (metalTexture.ID == 0) { std::cerr << "ERROR::TEXTURE: metalTexture failed to load. Check file path and integrity." << std::endl; }

    // Tell the objectShader that its "tex0" sampler should use texture unit 0.
    // This only needs to be done once after the shader is created and activated.
    objectShader.Activate();
    // The Texture::texUnit method typically ensures the shader is active and sets the uniform.
    // If not, you'd do: glUniform1i(glGetUniformLocation(objectShader.ID, "tex0"), 0);
    if (woodTexture.ID != 0) { // Use any valid texture to set the uniform for the shader
        woodTexture.texUnit(objectShader, "tex0", 0);
    }
    else if (brickTexture.ID != 0) {
        brickTexture.texUnit(objectShader, "tex0", 0);
    }
    else if (metalTexture.ID != 0) {
        metalTexture.texUnit(objectShader, "tex0", 0);
    }
    else {
        std::cerr << "WARNING: No valid textures to set 'tex0' sampler uniform for objectShader." << std::endl;
    }


    // --- Scene Objects (using smart pointers for automatic cleanup) ---
    std::vector<std::unique_ptr<Shape>> sceneObjects;

    // Mirror Plane (will also be drawn as a normal object)
    auto mirrorPlane = std::make_unique<Plane>(8.0f, 8.0f, glm::vec3(0.7f, 0.7f, 0.8f), glm::vec2(2.0f, 2.0f), -0.01f);
    mirrorPlane->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
    if (woodTexture.ID != 0) mirrorPlane->setTexture(&woodTexture);
    mirrorPlane->setupMesh();
    // Not adding to sceneObjects yet if we want to draw it separately always. Or add it and identify.
    // For simplicity in stencil, let's keep it separate, but it needs to be drawn in the main pass too.

    // Main Pyramid
    auto mainPyramid = std::make_unique<Pyramid>();
    mainPyramid->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.75f, 0.0f)); // y = 0.75f for 1.5f scale
    mainPyramid->modelMatrix = glm::scale(mainPyramid->modelMatrix, glm::vec3(1.5f));
    if (brickTexture.ID != 0) mainPyramid->setTexture(&brickTexture);
    mainPyramid->setupMesh();
    sceneObjects.push_back(std::move(mainPyramid));

    // A Cube
    auto cube1 = std::make_unique<Cube>(1.0f, 1.0f, 1.0f, glm::vec3(0.2f, 0.3f, 0.8f));
    cube1->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-3.0f, 0.5f, 1.5f));
    if (metalTexture.ID != 0) cube1->setTexture(&metalTexture); // Cube uses metal
    cube1->setupMesh();
    sceneObjects.push_back(
        std::move(cube1));

    // Another Pyramid, smaller and different position
    auto pyramid2 = std::make_unique<Pyramid>(glm::vec3(0.5f, 0.2f, 0.2f), glm::vec3(0.7f, 0.4f, 0.4f)); // Reddish
    pyramid2->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, 0.25f, -2.0f)); // y = 0.25f for 0.5f scale
    pyramid2->modelMatrix = glm::scale(pyramid2->modelMatrix, glm::vec3(0.5f));
    pyramid2->modelMatrix = glm::rotate(pyramid2->modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    if (woodTexture.ID != 0) pyramid2->setTexture(&woodTexture); // Small pyramid uses wood
    pyramid2->setupMesh();
    sceneObjects.push_back(std::move(pyramid2));

    // Ground Plane (distinct from mirror)
    auto groundPlane = std::make_unique<Plane>(25.0f, 25.0f, glm::vec3(0.3f, 0.5f, 0.2f), glm::vec2(12.0f, 12.0f), 0.0f);
    groundPlane->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.02f, 0.0f)); // Slightly below mirror
    if (metalTexture.ID != 0) groundPlane->setTexture(&metalTexture); // Ground uses metal (or a different "ground" texture)
    groundPlane->setupMesh();
    sceneObjects.push_back(std::move(groundPlane));


    // --- Light Source Object ---
    auto lightSourceCube = std::make_unique<Cube>(0.3f, 0.3f, 0.3f, glm::vec3(1.0f, 1.0f, 1.0f)); // White cube
    // Light source usually doesn't need a file-based texture, uses its vertex colors or a simple emissive color.
    lightSourceCube->setupMesh();

    // --- Lighting Properties ---
    glm::vec4 globalLightColor = glm::vec4(1.0f, 1.0f, 0.9f, 1.0f); // Slightly warm white light
    glm::vec3 animatedLightPos = glm::vec3(0.0f, 2.5f, 0.0f); // Initial light position

    // --- Reflection Setup ---
    const float mirrorPlaneY = -0.01f; // Y-coordinate of the mirror surface (make it const)
    glm::mat4 reflectionMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f * mirrorPlaneY, 0.0f)) *
        glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, -1.0f, 1.0f));


    auto mySphere = std::make_unique<Sphere>(1.0f, 32, 16, glm::vec3(0.8f, 0.2f, 0.2f)); // Promieñ 1, 32 sektory, 16 stosów
    if (metalTexture.ID != 0) mySphere->setTexture(&metalTexture);

    // Ustawienie macierzy modelu dla kuli (translacja, rotacja, skalowanie)
    mySphere->modelMatrix = glm::mat4(1.0f); // Zacznij od macierzy identycznoœciowej
    mySphere->modelMatrix = glm::translate(mySphere->modelMatrix, glm::vec3(-1.5f, 1.0f, -3.0f)); // Przesuñ kulê
    mySphere->modelMatrix = glm::rotate(mySphere->modelMatrix, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Obróæ o 45 stopni wokó³ osi Y
    mySphere->modelMatrix = glm::scale(mySphere->modelMatrix, glm::vec3(0.8f)); // Zmniejsz rozmiar

    mySphere->setupMesh();
    sceneObjects.push_back(std::move(mySphere));


    // Tworzenie Walca
    auto myCylinder = std::make_unique<Cylinder>(0.5f, 0.5f, 2.0f, 32, 4, true, glm::vec3(0.2f, 0.8f, 0.3f));
    // promieñ podstawy, promieñ góry, wysokoœæ, sektory, stosy, g³adkie cieniowanie
    if (brickTexture.ID != 0) myCylinder->setTexture(&brickTexture);

    myCylinder->modelMatrix = glm::mat4(1.0f);
    myCylinder->modelMatrix = glm::translate(myCylinder->modelMatrix, glm::vec3(2.0f, 1.0f, -2.5f));
    // Obrót walca, aby le¿a³ na boku (wokó³ osi X lub Z)
    myCylinder->modelMatrix = glm::rotate(myCylinder->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    // Dodatkowy obrót wokó³ jego nowej "pionowej" osi (która by³a pierwotnie Y)
    myCylinder->modelMatrix = glm::rotate(myCylinder->modelMatrix, glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));


    myCylinder->setupMesh();
    sceneObjects.push_back(std::move(myCylinder));

	 // Disable culling for the light source cube to ensure it's always visible

    // --- Render Loop ---
    while (!glfwWindowShouldClose(window)) {
        // --- Per-frame Time Logic ---
        float currentFrame = static_cast<float>(glfwGetTime()); // Use float for consistency
        // deltaTime can be calculated here if needed

        // --- Input ---
        camera.Inputs(window); // Process camera inputs

        // --- Update ---
        // Animate light source position
        float lightOrbitRadius = 4.0f;
        animatedLightPos.x = sin(currentFrame * 0.4f) * lightOrbitRadius;
        animatedLightPos.z = cos(currentFrame * 0.4f) * lightOrbitRadius;
        animatedLightPos.y = 2.0f + sin(currentFrame * 0.6f) * 1.5f;
        lightSourceCube->modelMatrix = glm::translate(glm::mat4(1.0f), animatedLightPos);

        // Update camera's view-projection matrix
        camera.updateMatrix(45.0f, 0.1f, 150.0f); // Increased far plane

        // --- Rendering ---
        glClearColor(0.02f, 0.02f, 0.05f, 1.0f); // Darker blue background
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // --- Set Common Shader Uniforms (once per shader type per frame if possible) ---
        // Object Shader (used for most scene elements)
        objectShader.Activate();
        camera.Matrix(objectShader, "camMatrix"); // VP matrix
        glUniform3fv(glGetUniformLocation(objectShader.ID, "lightPos"), 1, glm::value_ptr(animatedLightPos));
        glUniform3fv(glGetUniformLocation(objectShader.ID, "camPos"), 1, glm::value_ptr(camera.Position));
        glUniform4fv(glGetUniformLocation(objectShader.ID, "lightColor"), 1, glm::value_ptr(globalLightColor));
        // Individual textures will be bound by Shape::draw()

        // Light Shader (for the light source cube itself)
        lightShader.Activate();
        camera.Matrix(lightShader, "camMatrix"); // VP matrix
        glUniform4fv(glGetUniformLocation(lightShader.ID, "lightColor"), 1, glm::value_ptr(globalLightColor));


        // --- 1. Draw Mirror Surface to Stencil Buffer ---
        glEnable(GL_STENCIL_TEST);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE); // Don't write to color buffer
        glDisable(GL_DEPTH_TEST);                            // Stencil writing doesn't usually need depth
        glStencilFunc(GL_ALWAYS, 1, 0xFF);                   // Always pass, set stencil to 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);           // Replace stencil value if test passes
        glStencilMask(0xFF);                                 // Enable writing to stencil buffer
        // glClear(GL_STENCIL_BUFFER_BIT); // Already cleared at the start of the frame

        objectShader.Activate(); // Ensure correct shader for mirror
        mirrorPlane->draw(objectShader); // Shape::draw() handles its own texture

        // --- 2. Draw Reflected Scene (where stencil is 1) ---
        glEnable(GL_DEPTH_TEST);                             // Enable depth for reflected objects
        glDepthMask(GL_TRUE);                                // Allow writing to depth buffer
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);     // Enable writing to color buffer
        glStencilFunc(GL_EQUAL, 1, 0xFF);                    // Pass only if stencil value is 1
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);              // Keep stencil buffer contents the same
        glStencilMask(0x00);                                 // Disable writing to stencil buffer (important!)

        glCullFace(GL_FRONT); // Invert culling for reflected objects

        // Draw reflected scene objects
        objectShader.Activate(); // Ensure object shader is active
        for (const auto& obj : sceneObjects) {
            // Optionally skip drawing the ground plane if it's not meant to be reflected,
            // or if it's perfectly aligned with the mirror (which it isn't here).
            // if (obj.get() == groundPlane.get() && groundPlane->modelMatrix[3][1] < mirrorPlaneY) continue;

            glm::mat4 originalModel = obj->modelMatrix;
            obj->modelMatrix = reflectionMatrix * originalModel;
            obj->draw(objectShader); // Shape::draw() handles its own texture
            obj->modelMatrix = originalModel; // Restore original model matrix
        }

        // Draw reflected light source
        lightShader.Activate(); // Switch to light shader
        glm::mat4 originalLightModel = lightSourceCube->modelMatrix;
        lightSourceCube->modelMatrix = reflectionMatrix * originalLightModel;
        lightSourceCube->draw(lightShader);
        lightSourceCube->modelMatrix = originalLightModel; // Restore

        glCullFace(GL_BACK); // Restore default culling

        // --- 3. Draw Actual Scene (outside stencil area or after stencil clear) ---
        glDisable(GL_STENCIL_TEST); // Disable stencil test for the rest of the scene
        // glStencilMask(0xFF); // Allow clearing stencil for next frame (already default)

        // Draw the actual mirror plane (now visible)
        objectShader.Activate(); // Ensure object shader is active for the mirror
        // Potentially enable blending here if the mirror is semi-transparent
        // glEnable(GL_BLEND); glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        mirrorPlane->draw(objectShader); // Shape::draw() handles its own texture
        // glDisable(GL_BLEND);

        // Draw actual scene objects
        // objectShader is already active
        glDisable(GL_CULL_FACE);
        for (const auto& obj : sceneObjects) {
            obj->draw(objectShader); // Shape::draw() handles its own texture
        }
        glEnable(GL_CULL_FACE);

        // Draw actual light source
        lightShader.Activate(); // Switch to light shader
        lightSourceCube->draw(lightShader);


        // --- Swap Buffers and Poll Events ---
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    // Smart pointers (sceneObjects, lightSourceCube, mirrorPlane) will automatically call
    // their Shape destructors, which in turn call Shape::cleanup() to delete VAO/VBO/EBOs.
    sceneObjects.clear();
    // mirrorPlane, lightSourceCube unique_ptrs go out of scope and clean up.

    woodTexture.Delete();   // Explicitly delete textures
    brickTexture.Delete();
    metalTexture.Delete();

    objectShader.Delete();  // Explicitly delete shaders
    lightShader.Delete();

    glfwDestroyWindow(window); // Destroy window before terminating GLFW
    glfwTerminate();
    return 0;
}