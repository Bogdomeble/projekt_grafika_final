// main.cpp - Art Gallery Demo (Compatible with new multi-light shader, using 1 light, original rotations)
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>
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
#include "light.h"

// Constants
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
const float globalScale = 0.6f; // Global scale factor for all objects

int main() {
    srand(static_cast<unsigned int>(time(0))); // Initialize random seed

    // --- GLFW and GLAD Initialization ---
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Art Gallery - Single Light (Multi-Light Shader)", NULL, NULL);
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
    Shader objectShader("default.vert", "default.frag"); // Uses the shader prepared for multiple lights
    Shader lightSourceShader("light.vert", "light.frag");

    // --- Camera ---
    Camera camera(SCR_WIDTH, SCR_HEIGHT, glm::vec3(-0.100214, 1.61599, 5.2313));

    // --- Textures ---
    Texture floorTexture("floor_wood.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture wallTexture("brick4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture metalTexture("metal_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture WorldTexture("world.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    Texture artTexture1("art1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture2("art2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture3("art3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture4("art4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture5("art5.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture6("art6.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture7("art7.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture8("art8.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture9("art9.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture10("art10.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture11("art11.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    // Check if textures loaded (abbreviated)
    if (floorTexture.ID == 0) std::cerr << "Warning: floorTexture failed to load." << std::endl;
    // ... (rest of checks, assuming they are present and correct)
    if (artTexture9.ID == 0) std::cerr << "Warning: artTexture9 (formerly artTexturePlaster) failed to load." << std::endl;

    objectShader.Activate();
    if (floorTexture.ID != 0) floorTexture.texUnit(objectShader, "tex0", 0);
    else if (wallTexture.ID != 0) wallTexture.texUnit(objectShader, "tex0", 0);
    else std::cerr << "WARNING: No valid textures to set 'tex0' sampler uniform for objectShader." << std::endl;

    // --- Gallery Structure ---
    std::vector<std::unique_ptr<Shape>> galleryWalls;
    std::vector<std::unique_ptr<Shape>> artworks;
    std::vector<std::unique_ptr<Shape>> otherObjects;

    float galleryWidth = 10.0f;
    float galleryDepth = 12.0f;
    float galleryHeight = 4.0f;
    float artDisplayHeight = galleryHeight / 2.0f - 0.2f;

    // Floor & Ceiling
    auto floor_obj = std::make_unique<Plane>(galleryWidth, galleryDepth, glm::vec3(1.0f), glm::vec2(5.0f, 6.0f)); // Renamed variable from 'floor' to 'floor_obj'
    if (floorTexture.ID != 0) floor_obj->setTexture(&floorTexture);
    floor_obj->setupMesh();
    otherObjects.push_back(std::move(floor_obj));

    auto ceiling = std::make_unique<Plane>(galleryWidth, galleryDepth, glm::vec3(1.0f), glm::vec2(5.0f, 6.0f));
    ceiling->modelMatrix = glm::translate(ceiling->modelMatrix, glm::vec3(0.0f, galleryHeight, 0.0f));
    ceiling->modelMatrix = glm::rotate(ceiling->modelMatrix, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    if (floorTexture.ID != 0) ceiling->setTexture(&floorTexture);
    ceiling->setupMesh();
    otherObjects.push_back(std::move(ceiling));

    // Walls (original lambda createWall and its calls)
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

    // --- Artworks --- (original lambda addArt and its calls)
    float artHeightDefault = 1.5f;
    float artWidthDefault = 1.0f;
    float artDepthOffset = 0.051f;

    auto addArt = [&](float width, float height, Texture& texture, glm::vec3 translation, const std::vector<std::pair<float, glm::vec3>>& rotations) {
        auto art = std::make_unique<Plane>(width, height, glm::vec3(1.0f), glm::vec2(1.0f));
        if (texture.ID != 0) art->setTexture(&texture);
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        for (size_t i = 0; i < rotations.size(); ++i)
            model = glm::rotate(model, glm::radians(rotations[i].first), rotations[i].second);
        art->modelMatrix = model;
        art->setupMesh();
        artworks.push_back(std::move(art));
        };

    addArt(artWidthDefault, artHeightDefault, artTexture1,
        { -2.0f, artDisplayHeight, -galleryDepth / 2.0f + artDepthOffset },
        { {90.0f, {1.0f, 0.0f, 0.0f}} });

    addArt(artHeightDefault * 6.5f, artWidthDefault * 3.0f, artTexture2,
        { -galleryWidth / 2.0f + artDepthOffset, artDisplayHeight + 0.2f, 0.0f },
        { {90.0f, {1.0f, 0.0f, 0.0f}}, {-90.0f, {0.0f, 0.0f, 1.0f}}, {180.0f, {0.0f, 1.0f, 0.0f}} });

    addArt(artWidthDefault, artHeightDefault, artTexture3,
        { 0.0f, artDisplayHeight, -galleryDepth / 2.0f + artDepthOffset },
        { {90.0f, {1.0f, 0.0f, 0.0f}}, {180.0f, {0.0f, 1.0f, 0.0f}} });

    addArt(artWidthDefault * 1.2f, artHeightDefault * 0.8f, artTexture4,
        { 2.5f, artDisplayHeight, -galleryDepth / 2.0f + artDepthOffset },
        { {90.0f, {1.0f, 0.0f, 0.0f}}, {180.0f, {0.0f, 1.0f, 0.0f}} });

    addArt(artHeightDefault, artWidthDefault, artTexture5,
        { galleryWidth / 2.0f - artDepthOffset, artDisplayHeight, 0.0f },
        { {90.0f, {1.0f, 0.0f, 0.0f}}, {90.0f, {0.0f, 0.0f, 1.0f}}, {180.0f, {0.0f, 1.0f, 0.0f}} });

    addArt(artHeightDefault * 1.2f, artWidthDefault * 1.2f, artTexture6,
        { galleryWidth / 2.0f - artDepthOffset, artDisplayHeight, -3.0f },
        { {90.0f, {1.0f, 0.0f, 0.0f}}, {90.0f, {0.0f, 0.0f, 1.0f}}, {180.0f, {0.0f, 1.0f, 0.0f}} });

    addArt(artWidthDefault, artHeightDefault, artTexture7,
        { -2.0f, artDisplayHeight, galleryDepth / 2.0f - artDepthOffset },
        { {-90.0f, {1.0f, 0.0f, 0.0f}} });

    addArt(artWidthDefault, artHeightDefault, artTexture8,
        { 0.0f, artDisplayHeight, galleryDepth / 2.0f - artDepthOffset },
        { {-90.0f, {1.0f, 0.0f, 0.0f}} });

    addArt(artWidthDefault, artHeightDefault, artTexture9,
        { 2.0f, artDisplayHeight, galleryDepth / 2.0f - artDepthOffset },
        { {-90.0f, {1.0f, 0.0f, 0.0f}} });

    addArt(artHeightDefault, artWidthDefault, artTexture11,
        { galleryWidth / 2.0f - artDepthOffset, artDisplayHeight, 3.0f },
        { {90.0f, {1.0f, 0.0f, 0.0f}}, {90.0f, {0.0f, 0.0f, 1.0f}}, {180.0f, {0.0f, 1.0f, 0.0f}} });

    // --- Sculpture --- (original code)
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
    sculpturePtr->modelMatrix = glm::rotate(glm::mat4(sculpturePtr->modelMatrix), glm::radians(-90.0f), glm::vec3(0.0, 0.0, 1.0));
    sculpturePtr->setupMesh();
    otherObjects.push_back(std::move(sculpture_temp));

    glm::vec3 pedestal2Position = glm::vec3(-2.5f, 0.5f, -1.5f); // New position for the second pedestal
    auto pedestal2 = std::make_unique<Cylinder>(0.3f, 0.3f, 1.0f, 24, 1, true, glm::vec3(0.3f, 0.3f, 0.35f)); // Different pedestal color
    if (metalTexture.ID != 0) pedestal2->setTexture(&metalTexture); // You can use the same or a different texture
    pedestal2->modelMatrix = glm::translate(glm::mat4(1.0f), pedestal2Position);
    pedestal2->setupMesh();
    otherObjects.push_back(std::move(pedestal2));

    glm::vec3 pyramidPosition = glm::vec3(pedestal2Position.x, pedestal2Position.y + 0.5f + 0.4f, pedestal2Position.z); // On the pedestal (pedestal height 1.0/2 + pyramid height 0.8/2)
    auto pyramidSculpture = std::make_unique<Pyramid>(glm::vec3(0.7f, 0.2f, 0.2f), glm::vec3(0.9f, 0.5f, 0.5f)); // Pyramid colors

    if (artTexture10.ID != 0) pyramidSculpture->setTexture(&artTexture10);
    pyramidSculpture->setupMesh();
    Pyramid* pyramidPtr = pyramidSculpture.get();
    // Store a pointer to the pyramid
    otherObjects.push_back(std::move(pyramidSculpture));

    // --- Light Source (using PointLightData from light.h for a single light) ---
    PointLightData mainLight( // Name changed from pointLight to mainLight for clarity
        glm::vec3(0.0f, galleryHeight - 0.1f, 0.0f), // Initial position
        glm::vec4(0.9f, 0.9f, 0.8f, 1.0f)           // Color
    );
    // mainLight.visualRepresentation is created in the PointLightData constructor

    // --- Render Loop ---
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = static_cast<float>(glfwGetTime());

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Animate light
        mainLight.position.x = sin(currentFrame * 0.3f) * 3.0f;
        mainLight.position.z = cos(currentFrame * 0.3f) * 3.0f;
        // Update model matrix for light visualization


        // Animate sculpture
        if (sculpturePtr) {
            glm::mat4 initialTranslation = glm::translate(glm::mat4(1.0f), sculptureBasePosition);
            float rotationSpeed = 1.5f;
            glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), rotationSpeed * currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));
            sculpturePtr->modelMatrix = initialTranslation * rotationMatrix;
        }
        if (mainLight.visualRepresentation) {
            mainLight.visualRepresentation->modelMatrix = glm::translate(glm::mat4(1.0f), mainLight.position);
        }

        if (pyramidPtr) {
            glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(globalScale));
            glm::mat4 initialOrientationMat = glm::rotate(glm::mat4(1.0f), glm::radians(12.0f), glm::vec3(1.0f, 1.0f, 1.0f)); // Static initial orientation

            // Pyramid's own rotation (around its local Y axis, after initialOrientationMat)
            float spinSpeed = 2.5f;
            glm::mat4 spinRotation = glm::rotate(glm::mat4(1.0f), spinSpeed * currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));

            // Precession rotation - the entire system (already rotating pyramid) rotates around the global Y axis
            float precessionSpeed = 0.5f;
            glm::mat4 precessionRotation = glm::rotate(glm::mat4(1.0f), precessionSpeed * currentFrame, glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), pyramidPosition);

            // Order: T * R_precession * R_own_rotation * R_initial_orientation * S
            pyramidPtr->modelMatrix = translationMat * precessionRotation * spinRotation * initialOrientationMat * scaleMat;
        }

        glClearColor(0.05f, 0.86f, 0.86f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // --- Set Uniforms for Object Shader ---
        objectShader.Activate();
        camera.Matrix(objectShader, "camMatrix");
        glUniform3fv(glGetUniformLocation(objectShader.ID, "camPos"), 1, glm::value_ptr(camera.Position));

        // Send the data of ONE light as the first in the shader's array
        glUniform1i(glGetUniformLocation(objectShader.ID, "numActiveLights"), 1); // Only one active light
        glUniform3fv(glGetUniformLocation(objectShader.ID, "pointLights[0].position"), 1, glm::value_ptr(mainLight.position));
        glUniform4fv(glGetUniformLocation(objectShader.ID, "pointLights[0].color"), 1, glm::value_ptr(mainLight.color));

        // --- Draw Gallery Objects ---
        for (const auto& wall : galleryWalls) wall->draw(objectShader);
        for (const auto& art : artworks) art->draw(objectShader);
        for (const auto& obj : otherObjects) {
            bool isCylinder = (dynamic_cast<Cylinder*>(obj.get()) != nullptr);
            if (isCylinder) glDisable(GL_CULL_FACE);
            obj->draw(objectShader);
            if (isCylinder) glEnable(GL_CULL_FACE);
        }

        // --- Draw Light Source Visual ---
        lightSourceShader.Activate();
        camera.Matrix(lightSourceShader, "camMatrix");
        // Draw light visualization, if it exists
        if (mainLight.visualRepresentation) {
            glUniform4fv(glGetUniformLocation(lightSourceShader.ID, "lightColor"), 1, glm::value_ptr(mainLight.color));
            mainLight.visualRepresentation->draw(lightSourceShader);
        }

        //camera.printData(); // Original printData call

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    galleryWalls.clear();
    artworks.clear();
    otherObjects.clear();
    // mainLight.visualRepresentation will be automatically released by unique_ptr

    floorTexture.Delete();
    wallTexture.Delete();
    metalTexture.Delete();
    WorldTexture.Delete();

    artTexture1.Delete(); artTexture2.Delete(); artTexture3.Delete();
    artTexture4.Delete(); artTexture5.Delete(); artTexture6.Delete();
    artTexture7.Delete(); artTexture8.Delete(); artTexture9.Delete();

    objectShader.Delete();
    lightSourceShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}