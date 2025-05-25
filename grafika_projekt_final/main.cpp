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
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main() {
    // --- GLFW and GLAD Initialization ---
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Art Gallery ", NULL, NULL);
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

    // --- Shaders ---
    Shader objectShader("default.vert", "default.frag");
    Shader lightSourceShader("light.vert", "light.frag");

    // --- Camera ---
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(-0.100214, 1.61599, 5.2313));

    // --- Textures ---
    // Existing textures
    Texture floorTexture("floor_wood.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture wallTexture("brick4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture1("art1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture2("art2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture metalTexture("metal_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture WorldTexture("world.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    // New textures for additional artworks
    Texture artTexture3("art3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTextureBrick1("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTextureBrick3("brick3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTextureMarble("marble.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTextureMetal1("metal.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTextureMetal3("metal3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexturePlaster("wall_plaster.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);


    // Check if textures loaded
    if (floorTexture.ID == 0) std::cerr << "Warning: floorTexture failed to load." << std::endl;
    if (wallTexture.ID == 0) std::cerr << "Warning: wallTexture failed to load." << std::endl;
    if (artTexture1.ID == 0) std::cerr << "Warning: artTexture1 failed to load." << std::endl;
    if (artTexture2.ID == 0) std::cerr << "Warning: artTexture2 failed to load." << std::endl;
    if (metalTexture.ID == 0) std::cerr << "Warning: metalTexture failed to load." << std::endl;
    if (WorldTexture.ID == 0) std::cerr << "Warning: WorldTexture failed to load." << std::endl;
    if (artTexture3.ID == 0) std::cerr << "Warning: artTexture3 failed to load." << std::endl;
    if (artTextureBrick1.ID == 0) std::cerr << "Warning: artTextureBrick1 failed to load." << std::endl;
    if (artTextureBrick3.ID == 0) std::cerr << "Warning: artTextureBrick3 failed to load." << std::endl;
    if (artTextureMarble.ID == 0) std::cerr << "Warning: artTextureMarble failed to load." << std::endl;
    if (artTextureMetal1.ID == 0) std::cerr << "Warning: artTextureMetal1 failed to load." << std::endl;
    if (artTextureMetal3.ID == 0) std::cerr << "Warning: artTextureMetal3 failed to load." << std::endl;
    if (artTexturePlaster.ID == 0) std::cerr << "Warning: artTexturePlaster failed to load." << std::endl;


    objectShader.Activate();
    if (floorTexture.ID != 0) {
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
    float artDisplayHeight = galleryHeight / 2.0f - 0.2f; // Consistent Y for art

    // Floor & Ceiling
    auto floor = std::make_unique<Plane>(galleryWidth, galleryDepth, glm::vec3(1.0f), glm::vec2(5.0f, 6.0f));
    if (floorTexture.ID != 0) floor->setTexture(&floorTexture);
    floor->setupMesh();
    otherObjects.push_back(std::move(floor));

    auto ceiling = std::make_unique<Plane>(galleryWidth, galleryDepth, glm::vec3(1.0f), glm::vec2(5.0f, 6.0f));
    ceiling->modelMatrix = glm::translate(ceiling->modelMatrix, glm::vec3(0.0f, galleryHeight, 0.0f));
    ceiling->modelMatrix = glm::rotate(ceiling->modelMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (floorTexture.ID != 0) ceiling->setTexture(&floorTexture); // Assuming same texture for ceiling
    ceiling->setupMesh();
    otherObjects.push_back(std::move(ceiling));

    // Walls
    auto createWall = [&](const glm::vec3& position, const glm::vec3& rotation, float width, float height, glm::vec2 texRepeat) {
    auto wall = std::make_unique<Plane>(width, height, glm::vec3(0.8f), texRepeat);
    if (wallTexture.ID != 0) wall->setTexture(&wallTexture);
    wall->modelMatrix = glm::translate(glm::mat4(1.0f), position);
    wall->modelMatrix = glm::rotate(wall->modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    wall->modelMatrix = glm::rotate(wall->modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    wall->modelMatrix = glm::rotate(wall->modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    wall->setupMesh();
    galleryWalls.push_back(std::move(wall));
    };

    createWall(glm::vec3(0.0f, galleryHeight / 2.0f, -galleryDepth / 2.0f), glm::vec3(90.0f, 0.0f, 0.0f), galleryWidth, galleryHeight, glm::vec2(5.0f, 2.0f));
    createWall(glm::vec3(-galleryWidth / 2.0f, galleryHeight / 2.0f, 0.0f), glm::vec3(90.0f, 180.0f, 90.0f), galleryDepth, galleryHeight, glm::vec2(6.0f, 2.0f));
    createWall(glm::vec3(galleryWidth / 2.0f, galleryHeight / 2.0f, 0.0f), glm::vec3(90.0f, 180.0f, -90.0f), galleryDepth, galleryHeight, glm::vec2(6.0f, 2.0f));
    createWall(glm::vec3(0.0f, galleryHeight / 2.0f, galleryDepth / 2.0f), glm::vec3(90.0f, 180.0f, 180.0f), galleryWidth, galleryHeight, glm::vec2(5.0f, 2.0f));

    // --- Artworks ---
    float artHeightDefault = 1.5f; float artWidthDefault = 1.0f; float artDepthOffset = 0.051f; // Slightly more offset

    // Existing Art 1 (Back Wall)
    auto art1 = std::make_unique<Plane>(artWidthDefault, artHeightDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTexture1.ID != 0) art1->setTexture(&artTexture1);
    art1->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, artDisplayHeight, -galleryDepth / 2.0f + artDepthOffset));
    art1->modelMatrix = glm::rotate(art1->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    art1->setupMesh();
    artworks.push_back(std::move(art1));

    // Existing Art 2 (Left Wall) - Made it wider than tall
    auto art2 = std::make_unique<Plane>(artHeightDefault, artWidthDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTexture2.ID != 0) art2->setTexture(&artTexture2);
    art2->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-galleryWidth / 2.0f + artDepthOffset, artDisplayHeight + 0.2f, 0.0f)); // Slightly higher
    art2->modelMatrix = glm::rotate(art2->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    art2->modelMatrix = glm::rotate(art2->modelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    art2->setupMesh();
    artworks.push_back(std::move(art2));

    // New Artworks
    // Art 3 (art3.png) - Back Wall, center
    auto newArt1 = std::make_unique<Plane>(artWidthDefault, artHeightDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTexture3.ID != 0) newArt1->setTexture(&artTexture3);
    newArt1->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, artDisplayHeight, -galleryDepth / 2.0f + artDepthOffset));
    newArt1->modelMatrix = glm::rotate(newArt1->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    newArt1->modelMatrix = glm::rotate(newArt1->modelMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    newArt1->setupMesh();
    artworks.push_back(std::move(newArt1));

    // Art 4 (brick.png) - Back Wall, right
    auto newArt2 = std::make_unique<Plane>(artWidthDefault * 1.2f, artHeightDefault * 0.8f, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f)); // Different aspect
    if (artTextureBrick1.ID != 0) newArt2->setTexture(&artTextureBrick1);
    newArt2->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.5f, artDisplayHeight, -galleryDepth / 2.0f + artDepthOffset));
    newArt2->modelMatrix = glm::rotate(newArt2->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    newArt2->setupMesh();
    artworks.push_back(std::move(newArt2));

    // Art 5 (brick3.png) - Right Wall, center
    auto newArt3 = std::make_unique<Plane>(artHeightDefault, artWidthDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f)); // Portrait on side wall
    if (artTextureBrick3.ID != 0) newArt3->setTexture(&artTextureBrick3);
    newArt3->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(galleryWidth / 2.0f - artDepthOffset, artDisplayHeight, 0.0f));
    newArt3->modelMatrix = glm::rotate(newArt3->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    newArt3->modelMatrix = glm::rotate(newArt3->modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotated for right wall
    newArt3->setupMesh();
    artworks.push_back(std::move(newArt3));

    // Art 6 (marble.png) - Right Wall, towards back
    auto newArt4 = std::make_unique<Plane>(artHeightDefault * 1.2f, artWidthDefault * 1.2f, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f)); // Square-ish
    if (artTextureMarble.ID != 0) newArt4->setTexture(&artTextureMarble);
    newArt4->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(galleryWidth / 2.0f - artDepthOffset, artDisplayHeight, -3.0f));
    newArt4->modelMatrix = glm::rotate(newArt4->modelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    newArt4->modelMatrix = glm::rotate(newArt4->modelMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    newArt4->setupMesh();
    artworks.push_back(std::move(newArt4));

    // Art 7 (metal.png) - Front Wall, left
    auto newArt5 = std::make_unique<Plane>(artWidthDefault, artHeightDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTextureMetal1.ID != 0) newArt5->setTexture(&artTextureMetal1);
    newArt5->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, artDisplayHeight, galleryDepth / 2.0f - artDepthOffset));
    newArt5->modelMatrix = glm::rotate(newArt5->modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotated for front wall
    newArt5->setupMesh();
    artworks.push_back(std::move(newArt5));

    // Art 8 (metal3.png) - Front Wall, center
    auto newArt6 = std::make_unique<Plane>(artWidthDefault, artHeightDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTextureMetal3.ID != 0) newArt6->setTexture(&artTextureMetal3);
    newArt6->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, artDisplayHeight, galleryDepth / 2.0f - artDepthOffset));
    newArt6->modelMatrix = glm::rotate(newArt6->modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    newArt6->setupMesh();
    artworks.push_back(std::move(newArt6));

    // Art 9 (wall_plaster.png) - Front Wall, right
    auto newArt7 = std::make_unique<Plane>(artWidthDefault, artHeightDefault, glm::vec3(1.0f), glm::vec2(1.0f, 1.0f));
    if (artTexturePlaster.ID != 0) newArt7->setTexture(&artTexturePlaster);
    newArt7->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, artDisplayHeight, galleryDepth / 2.0f - artDepthOffset));
    newArt7->modelMatrix = glm::rotate(newArt7->modelMatrix, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    newArt7->setupMesh();
    artworks.push_back(std::move(newArt7));

    // --- Sculpture ---
    auto pedestal = std::make_unique<Cylinder>(0.3f, 0.3f, 1.0f, 24, 1, true, glm::vec3(0.4f));
    if (metalTexture.ID != 0) pedestal->setTexture(&metalTexture);
    pedestal->modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.5f, 0.5f, -1.0f));
    pedestal->setupMesh();
    otherObjects.push_back(std::move(pedestal));

    glm::vec3 sculptureBasePosition = glm::vec3(1.5f, 1.0f + 0.4f + 0.05f, -1.0f);
    auto sculpture_temp = std::make_unique<Sphere>(0.4f, 32, 16, glm::vec3(0.7f, 0.1f, 0.1f));
    Sphere* sculpturePtr = sculpture_temp.get();
    if (WorldTexture.ID != 0) sculpturePtr->setTexture(&WorldTexture);
    sculpturePtr->modelMatrix = glm::translate(glm::mat4(1.0f), sculptureBasePosition);
    sculpturePtr->setupMesh();
    otherObjects.push_back(std::move(sculpture_temp));

    // --- Light Source ---
    glm::vec3 pointLightPosition = glm::vec3(0.0f, galleryHeight - 0.1f, 0.0f);
    glm::vec4 pointLightColor = glm::vec4(0.9f, 0.9f, 0.8f, 1.0f);
    auto lightVisual = std::make_unique<Cube>(0.2f, 0.2f, 0.2f, glm::vec3(pointLightColor.r, pointLightColor.g, pointLightColor.b));
    lightVisual->setupMesh();


    // --- Render Loop ---
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Animate light
        pointLightPosition.x = sin(currentFrame * 0.3f) * 3.0f;
        pointLightPosition.z = cos(currentFrame * 0.3f) * 3.0f;
        lightVisual->modelMatrix = glm::translate(glm::mat4(1.0f), pointLightPosition);

        // Animate sculpture
        if (sculpturePtr) {
            glm::mat4 initialTranslation = glm::translate(glm::mat4(1.0f), sculptureBasePosition);
            float rotationSpeed = 0.5f;
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationSpeed * currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));
            sculpturePtr->modelMatrix = initialTranslation * rotationMatrix;
        }

        glClearColor(0.05f, 0.86f, 0.86f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Set Uniforms for Object Shader ---
        objectShader.Activate(); // Activate object shader before setting its uniforms
        camera.Matrix(objectShader, "camMatrix");
        glUniform3fv(glGetUniformLocation(objectShader.ID, "lightPos"), 1, glm::value_ptr(pointLightPosition));
        glUniform4fv(glGetUniformLocation(objectShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor));
        glUniform3fv(glGetUniformLocation(objectShader.ID, "camPos"), 1, glm::value_ptr(camera.Position));

        // --- Draw Gallery Objects ---
        for (const auto& wall : galleryWalls) {
            wall->draw(objectShader);
        }
        for (const auto& art : artworks) {
            art->draw(objectShader);
        }
        for (const auto& obj : otherObjects) {
            bool isCylinder = (dynamic_cast<Cylinder*>(obj.get()) != nullptr);
            if (isCylinder) {
                glDisable(GL_CULL_FACE);
            }
            obj->draw(objectShader);
            if (isCylinder) {
                glEnable(GL_CULL_FACE);
            }
        }

        // --- Draw Light Source Visual ---
        lightSourceShader.Activate();
        camera.Matrix(lightSourceShader, "camMatrix");
        glUniform4fv(glGetUniformLocation(lightSourceShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor)); // Pass light color to light shader
        lightVisual->draw(lightSourceShader);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    galleryWalls.clear();
    artworks.clear();
    otherObjects.clear();

    floorTexture.Delete();
    wallTexture.Delete();
    artTexture1.Delete();
    artTexture2.Delete();
    metalTexture.Delete();
    WorldTexture.Delete();

    // Delete new textures
    artTexture3.Delete();
    artTextureBrick1.Delete();
    artTextureBrick3.Delete();
    artTextureMarble.Delete();
    artTextureMetal1.Delete();
    artTextureMetal3.Delete();
    artTexturePlaster.Delete();

    objectShader.Delete();
    lightSourceShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
