// main.cpp - Art Gallery Demo (Compatible with user's provided shaders)
#include <iostream>
#include <vector>
#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "texture.h"
#include "camera.h"
#include "Shape.h"
#include "Cube.h"
#include "Plane.h"
#include "Pyramid.h"
#include "Sphere.h"
#include "Cylinder.h"

// Constants
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

int main() {
    // --- GLFW and GLAD Initialization ---
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // No stencil buffer needed for this version as reflections aren't implemented with these shaders

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Art Gallery (User Shaders)", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // --- Shaders (Using names from user's provided files) ---
    Shader objectShader("default.vert", "default.frag");
    Shader lightSourceShader("light.vert", "light.frag");

    // --- Camera ---
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(0.0f, 1.7f, 5.5f));

    // --- Textures ---
    Texture floorTexture("floor_wood.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture wallTexture("wall_plaster.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture1("art1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture2("art2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture metalTexture("metal_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    if (floorTexture.ID == 0) std::cerr << "Warning: floorTexture failed to load." << std::endl;
    if (wallTexture.ID == 0) std::cerr << "Warning: wallTexture failed to load." << std::endl;
    if (artTexture1.ID == 0) std::cerr << "Warning: artTexture1 failed to load." << std::endl;
    if (artTexture2.ID == 0) std::cerr << "Warning: artTexture2 failed to load." << std::endl;
    if (metalTexture.ID == 0) std::cerr << "Warning: metalTexture failed to load." << std::endl;


    // Set the "tex0" sampler uniform for the objectShader ONCE.
    // Shape::draw() will bind the correct Texture object to texture unit 0.
    objectShader.Activate();
    if (floorTexture.ID != 0) { // Use any valid texture to call texUnit
        floorTexture.texUnit(objectShader, "tex0", 0);
    }
    else {
        std::cerr << "WARNING: No valid textures to set 'tex0' sampler uniform for objectShader." << std::endl;
    }

    // --- Gallery Structure ---
    std::vector<std::unique_ptr<Shape>> galleryWalls;
    std::vector<std::unique_ptr<Shape>> artworks;
    std::vector<std::unique_ptr<Shape>> otherObjects;

    float galleryWidth = 10.0f;
    float galleryDepth = 12.0f;
    float galleryHeight = 4.0f;

    // Floor
    auto floor = std::make_unique<Plane>(galleryWidth, galleryDepth, glm::vec3(1.0f), glm::vec2(5.0f, 6.0f));
    if (floorTexture.ID != 0) floor->setTexture(&floorTexture);
    floor->setupMesh();
    otherObjects.push_back(std::move(floor));

    // Walls
    auto backWall = std::make_unique<Plane>(galleryWidth, galleryHeight, glm::vec3(0.8f), glm::vec2(5.0f, 2.0f));
    if (wallTexture.ID != 0) backWall->setTexture(&wallTexture);
    backWall->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, galleryHeight / 2.0f, -galleryDepth / 2.0f));
    backWall->modelMatrix = glm::rotate(backWall->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    backWall->setupMesh();
    galleryWalls.push_back(std::move(backWall));

    auto leftWall = std::make_unique<Plane>(galleryDepth, galleryHeight, glm::vec3(0.8f), glm::vec2(6.0f, 2.0f));
    if (wallTexture.ID != 0) leftWall->setTexture(&wallTexture);
    leftWall->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-galleryWidth / 2.0f, galleryHeight / 2.0f, 0.0f));
    leftWall->modelMatrix = glm::rotate(leftWall->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    leftWall->modelMatrix = glm::rotate(leftWall->modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    leftWall->setupMesh();
    galleryWalls.push_back(std::move(leftWall));

    auto rightWall = std::make_unique<Plane>(galleryDepth, galleryHeight, glm::vec3(0.8f), glm::vec2(6.0f, 2.0f));
    if (wallTexture.ID != 0) rightWall->setTexture(&wallTexture);
    rightWall->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(galleryWidth / 2.0f, galleryHeight / 2.0f, 0.0f));
    rightWall->modelMatrix = glm::rotate(rightWall->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    rightWall->modelMatrix = glm::rotate(rightWall->modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    rightWall->setupMesh();
    galleryWalls.push_back(std::move(rightWall));

    // --- Artworks ---
    float artHeight = 1.5f; float artWidth = 1.0f; float artDepthOffset = 0.05f;
    auto art1 = std::make_unique<Plane>(artWidth, artHeight, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTexture1.ID != 0) art1->setTexture(&artTexture1);
    art1->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, galleryHeight / 2.0f - 0.2f, -galleryDepth / 2.0f + artDepthOffset));
    art1->modelMatrix = glm::rotate(art1->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    art1->setupMesh();
    artworks.push_back(std::move(art1));

    auto art2 = std::make_unique<Plane>(artHeight, artWidth, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f)); // Wider than tall
    if (artTexture2.ID != 0) art2->setTexture(&artTexture2);
    art2->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-galleryWidth / 2.0f + artDepthOffset, galleryHeight / 2.0f, 0.0f));
    art2->modelMatrix = glm::rotate(art2->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    art2->modelMatrix = glm::rotate(art2->modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    art2->setupMesh();
    artworks.push_back(std::move(art2));

    // --- Sculpture ---
    auto pedestal = std::make_unique<Cylinder>(0.3f, 0.3f, 1.0f, 24, 1, true, glm::vec3(0.4f));
    if (metalTexture.ID != 0) pedestal->setTexture(&metalTexture);
    pedestal->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.5f, -1.0f));
    pedestal->setupMesh();
    otherObjects.push_back(std::move(pedestal));

    auto sculpture = std::make_unique<Sphere>(0.4f, 32, 16, glm::vec3(0.7f, 0.1f, 0.1f));
    if (metalTexture.ID != 0) sculpture->setTexture(&metalTexture);
    sculpture->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 1.0f + 0.4f + 0.05f, -1.0f));
    sculpture->setupMesh();
    otherObjects.push_back(std::move(sculpture));

    // --- Light Source (Single Point Light for these shaders) ---
    glm::vec3 pointLightPosition = glm::vec3(0.0f, galleryHeight - 0.5f, 0.0f); // Central ceiling light
    glm::vec4 pointLightColor = glm::vec4(0.9f, 0.9f, 0.8f, 1.0f); // Light color (vec4 for shader uniform)

    auto lightVisual = std::make_unique<Cube>(0.2f, 0.2f, 0.2f, glm::vec3(pointLightColor.r, pointLightColor.g, pointLightColor.b));
    // lightVisual->modelMatrix will be updated in the loop
    lightVisual->setupMesh();


    // --- Render Loop ---
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Animate light (optional)
        // pointLightPosition.x = sin(currentFrame * 0.3f) * 3.0f;
        // pointLightPosition.z = cos(currentFrame * 0.3f) * 3.0f;
        lightVisual->modelMatrix = glm::translate(glm::mat4(1.0f), pointLightPosition);


        glClearColor(0.05f, 0.05f, 0.08f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Set Uniforms for Object Shader ---
        objectShader.Activate();
        camera.Matrix(objectShader, "camMatrix"); // Set VP matrix
        glUniform3fv(glGetUniformLocation(objectShader.ID, "lightPos"), 1, glm::value_ptr(pointLightPosition));
        glUniform4fv(glGetUniformLocation(objectShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor));
        glUniform3fv(glGetUniformLocation(objectShader.ID, "camPos"), 1, glm::value_ptr(camera.Position));
        // The "tex0" sampler uniform is already set. Shape::draw will bind textures.

        // --- Draw Gallery Objects ---
        for (const auto& wall : galleryWalls) {
            wall->draw(objectShader);
        }
        for (const auto& art : artworks) {
            art->draw(objectShader);
        }
        for (const auto& obj : otherObjects) {
            // Workaround for cylinder culling if its winding order is not perfect
            bool isCylinder = (dynamic_cast<Cylinder*>(obj.get()) != nullptr);
            if (isCylinder) {
                glDisable(GL_CULL_FACE); // Temporarily disable culling for cylinder
            }
            obj->draw(objectShader);
            if (isCylinder) {
                glEnable(GL_CULL_FACE); // Re-enable culling
            }
        }

        // --- Draw Light Source Visual ---
        lightSourceShader.Activate();
        camera.Matrix(lightSourceShader, "camMatrix"); // Set VP matrix
        glUniform4fv(glGetUniformLocation(lightSourceShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor));
        lightVisual->draw(lightSourceShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    galleryWalls.clear();
    artworks.clear();
    otherObjects.clear();
    // lightVisual unique_ptr will clean up

    floorTexture.Delete();
    wallTexture.Delete();
    artTexture1.Delete();
    artTexture2.Delete();
    // artTexture3.Delete(); // If you had it
    metalTexture.Delete();

    objectShader.Delete();
    lightSourceShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}