#include "../include/camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Camera::Position = position;

    yaw = -90.0f;
    pitch = 0.0f;

    // Initialize last mouse positions to the center of the screen
    lastX = static_cast<float>(width) / 2.0f;
    lastY = static_cast<float>(height) / 2.0f;
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::lookAt(Position, Position + Orientation, Up);
    glm::mat4 projection = glm::perspective(glm::radians(FOVdeg),
                                            static_cast<float>(width) / static_cast<float>(height),
                                            nearPlane, farPlane);
    cameraMatrix = projection * view;
}

void Camera::Matrix(const Shader& shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window, float deltaTime)
{
    // --- Keyboard movement (Correct and Unchanged) ---
    float current_speed = speed;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        current_speed *= 4.0f;
    }
    float velocity = current_speed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) Position += Orientation * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) Position -= glm::normalize(glm::cross(Orientation, Up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) Position -= Orientation * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) Position += glm::normalize(glm::cross(Orientation, Up)) * velocity;
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) Position += Up * velocity;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) Position -= Up * velocity;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);


    // --- COMPLETELY REVISED MOUSE LOGIC ---
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // On the very first click, update lastX and lastY to the current mouse position
        // This prevents the camera from jumping to the center of the screen.
        if (firstClick)
        {
            lastX = static_cast<float>(mouseX);
            lastY = static_cast<float>(mouseY);
            firstClick = false;
        }

        // Calculate the offset since the last frame
        float xoffset = static_cast<float>(mouseX) - lastX;
        float yoffset = lastY - static_cast<float>(mouseY); // Reversed since y-coordinates go from top to bottom

        // Update last positions for the next frame
        lastX = static_cast<float>(mouseX);
        lastY = static_cast<float>(mouseY);

        // Apply sensitivity
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Clamp pitch
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        // Calculate and set the new orientation vector
        glm::vec3 newOrientation;
        newOrientation.x = cosf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        newOrientation.y = sinf(glm::radians(pitch));
        newOrientation.z = sinf(glm::radians(yaw)) * cosf(glm::radians(pitch));
        Orientation = glm::normalize(newOrientation);
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Reset firstClick flag so the next click will capture the new starting mouse position
        firstClick = true;
    }
}