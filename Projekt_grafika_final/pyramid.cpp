#include "Pyramid.h"
// #include <glm/gtx/string_cast.hpp> // Uncomment for debugging

Pyramid::Pyramid(const glm::vec3& bColor, const glm::vec3& pColor) : baseColor(bColor), peakColor(pColor) {
    Type = ShapeType::SHAPE_TYPE_PYRAMID; // Ensure the type is set to pyramid
}

void Pyramid::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    // Define the vertices of the pyramid
    glm::vec3 p_peak = { 0.0f, 0.8f, 0.0f };  // Top vertex (peak)
    glm::vec3 p_blf = { -0.5f, 0.0f,  0.5f }; // Base: Bottom-Left-Front
    glm::vec3 p_brf = { 0.5f, 0.0f,  0.5f }; // Base: Bottom-Right-Front
    glm::vec3 p_blb = { -0.5f, 0.0f, -0.5f }; // Base: Bottom-Left-Back
    glm::vec3 p_brb = { 0.5f, 0.0f, -0.5f }; // Base: Bottom-Right-Back

    // Texture coordinates for the sides of the pyramid
    // Base corners of the side triangle: (0,0) and (1,0)
    // Top vertex of the side triangle: (0.5,1)
    glm::vec2 tc_side_base_left = { 0.0f, 0.0f };
    glm::vec2 tc_side_base_right = { 1.0f, 0.0f };
    glm::vec2 tc_side_peak_center = { 0.5f, 1.0f };

    // Texture coordinates for the base (square)
    glm::vec2 tc_base_00 = { 0.0f, 0.0f }; // e.g., for p_blb
    glm::vec2 tc_base_10 = { 1.0f, 0.0f }; // e.g., for p_brb
    glm::vec2 tc_base_11 = { 1.0f, 1.0f }; // e.g., for p_brf
    glm::vec2 tc_base_01 = { 0.0f, 1.0f }; // e.g., for p_blf

    GLuint current_vertex_index = 0; // Counter for added vertices

    // --- Base (normal pointing downwards: 0, -1, 0) ---
    // Vertices added in a CCW order when viewed from below
    // (i.e., if you look at the base from the side the normal "exits")
    glm::vec3 n_bottom = { 0.0f, -1.0f, 0.0f };
    // Order: p_blf, p_blb, p_brb, p_brf (CCW when viewed from below)
    addVertex(p_blf, baseColor, tc_base_01, n_bottom); // Index 0 in this section
    addVertex(p_blb, baseColor, tc_base_00, n_bottom); // Index 1
    addVertex(p_brb, baseColor, tc_base_10, n_bottom); // Index 2
    addVertex(p_brf, baseColor, tc_base_11, n_bottom); // Index 3

    indices_data.insert(indices_data.end(), { current_vertex_index + 0, current_vertex_index + 1, current_vertex_index + 2 });
    indices_data.insert(indices_data.end(), { current_vertex_index + 0, current_vertex_index + 2, current_vertex_index + 3 });
    current_vertex_index += 4; // Added 4 vertices for the base

    // --- Side faces (normals calculated, CCW winding order) ---

    // Front face: triangle (p_blf, p_brf, p_peak)
    // Normal: looking from the front, vector (p_brf - p_blf) x (p_peak - p_blf)
    glm::vec3 n_front_side = glm::normalize(glm::cross(p_brf - p_blf, p_peak - p_blf));
    addVertex(p_blf, baseColor, tc_side_base_left, n_front_side); // Index 4 (current_vertex_index + 0)
    addVertex(p_brf, baseColor, tc_side_base_right, n_front_side); // Index 5 (current_vertex_index + 1)
    addVertex(p_peak, peakColor, tc_side_peak_center, n_front_side); // Index 6 (current_vertex_index + 2)
    indices_data.insert(indices_data.end(), { current_vertex_index + 0, current_vertex_index + 1, current_vertex_index + 2 });
    current_vertex_index += 3;

    // Right face: triangle (p_brf, p_brb, p_peak)
    glm::vec3 n_right_side = glm::normalize(glm::cross(p_brb - p_brf, p_peak - p_brf));
    addVertex(p_brf, baseColor, tc_side_base_left, n_right_side); // Index 7
    addVertex(p_brb, baseColor, tc_side_base_right, n_right_side); // Index 8
    addVertex(p_peak, peakColor, tc_side_peak_center, n_right_side); // Index 9
    indices_data.insert(indices_data.end(), { current_vertex_index + 0, current_vertex_index + 1, current_vertex_index + 2 });
    current_vertex_index += 3;

    // Back face: triangle (p_brb, p_blb, p_peak)
    glm::vec3 n_back_side = glm::normalize(glm::cross(p_blb - p_brb, p_peak - p_brb));
    addVertex(p_brb, baseColor, tc_side_base_left, n_back_side);  // Index 10
    addVertex(p_blb, baseColor, tc_side_base_right, n_back_side);  // Index 11
    addVertex(p_peak, peakColor, tc_side_peak_center, n_back_side);  // Index 12
    indices_data.insert(indices_data.end(), { current_vertex_index + 0, current_vertex_index + 1, current_vertex_index + 2 });
    current_vertex_index += 3;

    // Left face: triangle (p_blb, p_blf, p_peak)
    glm::vec3 n_left_side = glm::normalize(glm::cross(p_blf - p_blb, p_peak - p_blb));
    addVertex(p_blb, baseColor, tc_side_base_left, n_left_side);  // Index 13
    addVertex(p_blf, baseColor, tc_side_base_right, n_left_side);  // Index 14
    addVertex(p_peak, peakColor, tc_side_peak_center, n_left_side);  // Index 15
    indices_data.insert(indices_data.end(), { current_vertex_index + 0, current_vertex_index + 1, current_vertex_index + 2 });
    // current_vertex_index += 3; // Not needed, this is the last group of vertices
}