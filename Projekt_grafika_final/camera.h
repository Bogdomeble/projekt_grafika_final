#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL
#include "shaderClass.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/quaternion.hpp>

class Camera {
public:
    // Camera attributes
    glm::mat4 cameraMatrix = glm::mat4(1.0f);
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstClick = true;
    int width;
    int height;

    float speed = 1.0f;
    float sensitivity = 100.0f;
    float deltaTime = glfwGetTime();

    Camera(int width, int height, glm::vec3 position);

    void printData();
    void Matrix(Shader& shader, const char* uniform);
    void Inputs(GLFWwindow* window);
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
};

#endif