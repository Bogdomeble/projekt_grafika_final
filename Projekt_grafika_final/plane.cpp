    #include "plane.h"

    Plane::Plane(float width, float length, const glm::vec3& color, const glm::vec2& texScale, float yOffset)
        : p_width(width), p_length(length), p_color(color), p_texScale(texScale), p_yOffset(yOffset) {
        Type = ShapeType::SHAPE_TYPE_PLANE; // Set the shape type to plane
    }

    void Plane::generateGeometry() {
        vertices_data.clear(); // Clear existing vertex data
        indices_data.clear();  // Clear existing index data

        float hw = p_width / 2.0f;  // Half width
        float hl = p_length / 2.0f; // Half length

        glm::vec3 normal(0.0f, 1.0f, 0.0f); // Assuming plane is on XZ plane, facing up along Y-axis

        // Define vertices for the plane
        glm::vec3 v0 = { -hw, p_yOffset, hl };  // Top-left
        glm::vec3 v1 = { hw, p_yOffset, hl };   // Top-right
        glm::vec3 v2 = { hw, p_yOffset, -hl };  // Bottom-right
        glm::vec3 v3 = { -hw, p_yOffset, -hl }; // Bottom-left

        // Define texture coordinates
        glm::vec2 tc0 = { 0.0f, p_texScale.y };          // Top-left texture coordinate
        glm::vec2 tc1 = { p_texScale.x, p_texScale.y };          // Top-right texture coordinate
        glm::vec2 tc2 = { p_texScale.x, 0.0f };          // Bottom-right texture coordinate
        glm::vec2 tc3 = { 0.0f, 0.0f };          // Bottom-left texture coordinate

        addVertex(v0, p_color, tc0, normal); // Vertex 0
        addVertex(v1, p_color, tc1, normal); // Vertex 1
        addVertex(v2, p_color, tc2, normal); // Vertex 2
        addVertex(v3, p_color, tc3, normal); // Vertex 3

        indices_data = {
            0, 1, 2, // First triangle
            0, 2, 3  // Second triangle
        };
    }