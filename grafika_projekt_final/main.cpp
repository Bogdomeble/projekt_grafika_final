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

int main()
{
    // --- GLFW and GLAD Initialization ---
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Art Gallery ", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
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
    Texture floorTexture("floor_wood.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture wallTexture("brick4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture metalTexture("metal_texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture WorldTexture("world.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);

    // Artworks textures
    Texture artTexture1("art1.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture2("art2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture3("art3.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture4("art4.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture5("art5.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture6("art6.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture7("art7.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture8("art8.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    Texture artTexture9("art9.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);


    // Check if textures loaded
    if (floorTexture.ID == 0) std::cerr << "Warning: floorTexture failed to load." << std::endl;
    if (wallTexture.ID == 0) std::cerr << "Warning: wallTexture failed to load." << std::endl;
    if (metalTexture.ID == 0) std::cerr << "Warning: metalTexture failed to load." << std::endl;
    if (WorldTexture.ID == 0) std::cerr << "Warning: WorldTexture failed to load." << std::endl;

    if (artTexture1.ID == 0) std::cerr << "Warning: artTexture1 failed to load." << std::endl;
    if (artTexture2.ID == 0) std::cerr << "Warning: artTexture2 failed to load." << std::endl;
    if (artTexture3.ID == 0) std::cerr << "Warning: artTexture3 failed to load." << std::endl;
    if (artTexture4.ID == 0) std::cerr << "Warning: artTexture4 failed to load." << std::endl;
    if (artTexture5.ID == 0) std::cerr << "Warning: artTexture5 failed to load." << std::endl;
    if (artTexture6.ID == 0) std::cerr << "Warning: artTexture6 failed to load." << std::endl;
    if (artTexture7.ID == 0) std::cerr << "Warning: artTexture7 failed to load." << std::endl;
    if (artTexture8.ID == 0) std::cerr << "Warning: artTexture8 failed to load." << std::endl;
    if (artTexture9.ID == 0) std::cerr << "Warning: artTexturePlaster failed to load." << std::endl;


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

    addArt(artHeightDefault, artWidthDefault, artTexture2,
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
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Animate light
        pointLightPosition.x = sin(currentFrame * 0.3f) * 3.0f;
        pointLightPosition.z = cos(currentFrame * 0.3f) * 3.0f;
        lightVisual->modelMatrix = glm::translate(glm::mat4(1.0f), pointLightPosition);

        // Animate sculpture
        if (sculpturePtr)
        {
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

        // ceiling light
        glUniform3fv(glGetUniformLocation(objectShader.ID, "lightPos"), 1, glm::value_ptr(pointLightPosition));
        glUniform4fv(glGetUniformLocation(objectShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor));

        // camera
        glUniform3fv(glGetUniformLocation(objectShader.ID, "camPos"), 1, glm::value_ptr(camera.Position));

        // --- Draw Gallery Objects ---
        for (const auto& wall : galleryWalls) wall->draw(objectShader);

        for (const auto& art : artworks) art->draw(objectShader);
        
        for (const auto& obj : otherObjects)
        {
            bool isCylinder = (dynamic_cast<Cylinder*>(obj.get()) != nullptr);
            if (isCylinder) glDisable(GL_CULL_FACE);
            obj->draw(objectShader);
            if (isCylinder) glEnable(GL_CULL_FACE);
        }

        // --- Draw Light Source Visual ---
        lightSourceShader.Activate();
        camera.Matrix(lightSourceShader, "camMatrix");

        glUniform4fv(glGetUniformLocation(lightSourceShader.ID, "lightColor"), 1, glm::value_ptr(pointLightColor)); // Pass light color to light shader
        lightVisual->draw(lightSourceShader);

        camera.printData();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // --- Cleanup ---
    galleryWalls.clear();
    artworks.clear();
    otherObjects.clear();

    floorTexture.Delete();
    wallTexture.Delete();
    metalTexture.Delete();
    WorldTexture.Delete();

    // Delete new textures
    artTexture1.Delete();
    artTexture2.Delete();
    artTexture3.Delete();
    artTexture4.Delete();
    artTexture5.Delete();
    artTexture6.Delete();
    artTexture7.Delete();
    artTexture8.Delete();
    artTexture9.Delete();

    objectShader.Delete();
    lightSourceShader.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
