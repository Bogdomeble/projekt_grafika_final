#include "plane.h"

Plane::Plane(float width, float length, const glm::vec3& color, const glm::vec2& texScale, float yOffset)
    : p_width(width), p_length(length), p_color(color), p_texScale(texScale), p_yOffset(yOffset) {
}

void Plane::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    float hw = p_width / 2.0f;
    float hl = p_length / 2.0f;

    glm::vec3 normal(0.0f, 1.0f, 0.0f); // Assuming plane is on XZ, facing up Y

    // Define vertices for the plane
    glm::vec3 v0 = { -hw, p_yOffset,  hl }; // Top-left
    glm::vec3 v1 = { hw, p_yOffset,  hl }; // Top-right
    glm::vec3 v2 = { hw, p_yOffset, -hl }; // Bottom-right
    glm::vec3 v3 = { -hw, p_yOffset, -hl }; // Bottom-left

    // Define texture coordinates
    glm::vec2 tc0 = { 0.0f, p_texScale.y };          // Match your original mirror plane
    glm::vec2 tc1 = { p_texScale.x, p_texScale.y };
    glm::vec2 tc2 = { p_texScale.x, 0.0f };
    glm::vec2 tc3 = { 0.0f, 0.0f };


    addVertex(v0, p_color, tc0, normal); // 0
    addVertex(v1, p_color, tc1, normal); // 1
    addVertex(v2, p_color, tc2, normal); // 2
    addVertex(v3, p_color, tc3, normal); // 3

    indices_data = {
        0, 1, 2,
        0, 2, 3
    };
}