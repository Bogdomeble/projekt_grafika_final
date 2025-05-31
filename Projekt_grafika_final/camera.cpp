#include "camera.h"
#include <GLFW/glfw3.h>
#include <Windows.h>

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(Shader& shader, const char* uniform)
{
    // Set the camera matrix uniform in the shader
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    // Initialize view and projection matrices
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Calculate the view matrix using lookAt
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Calculate the projection matrix using perspective
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Calculate and store the camera matrix (projection * view)
    cameraMatrix = projection * view;
}

void Camera::Inputs(GLFWwindow* window)
{
    float deltaTime = 0.10f;

    // Close window if ESC key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

    // Handle movement keys
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * Orientation * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += speed * -glm::normalize(glm::cross(Orientation, Up)) * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += speed * -Orientation * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * glm::normalize(glm::cross(Orientation, Up)) * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    {
        Position += speed * -Up * deltaTime;
    }

    // Handle speed modification with left shift
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = 0.4f * deltaTime;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = 0.1f * deltaTime;
    }

    // Handle mouse input for camera rotation
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        // Hide mouse cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // Prevent camera jump on first click
        if (firstClick)
        {
            glfwSetCursorPos(window, (width / 2), (height / 2));
            firstClick = false;
        }

        double mouseX;
        double mouseY;
        // Get mouse coordinates
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Calculate mouse movement offsets
        float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
        float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

        // Calculate the new orientation based on mouse input
        glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX),
            glm::normalize(glm::cross(Orientation, Up)));

        // Restrict vertical movement to prevent flipping
        if (abs(glm::angle(newOrientation, Up) - glm::radians(90.0f)) <=
            glm::radians(85.0f))
        {
            Orientation = newOrientation;
        }

        // Rotate the orientation horizontally
        Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

        // Reset mouse position to center
        glfwSetCursorPos(window, (width / 2), (height / 2));
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        // Show cursor when not rotating
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        // Reset first click flag
        firstClick = true;
    }
}

void Camera::printData() {
    // Get console handle (Windows-specific)
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    COORD cursorPos = csbi.dwCursorPosition;

    // Move cursor up 3 lines
    cursorPos.Y -= 3;
    SetConsoleCursorPosition(hConsole, cursorPos);

    // Overwrite previous data with spaces to clear any remaining characters
    std::cout << "Position: " << Position.x << ", " << Position.y << ", " << Position.z << "          \n";
    std::cout << "Orientation: " << Orientation.x << ", " << Orientation.y << ", " << Orientation.z << " \n";
    std::cout << "Speed & sensitivity: " << deltaTime*speed << " , " << sensitivity << "\n";

    // Flush the output stream to ensure immediate display
    std::cout.flush();
}