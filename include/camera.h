#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include "shaderClass.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    glm::vec3 Position{};
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;
    int width;
    int height;

    float speed = 5.0f;
    float sensitivity = 0.1f;

    float yaw;
    float pitch;

    // --- NEW: Variables to store the last mouse position ---
    float lastX;
    float lastY;

    Camera(int width, int height, glm::vec3 position);

    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    void Matrix(const Shader& shader, const char* uniform);
    void Inputs(GLFWwindow* window, float deltaTime);
};

#endif