#include "cube.h"

Cube::Cube(float w, float h, float d, const glm::vec3& color)
    : width(w), height(h), depth(d), faceColor(color) {
    // The Shape constructor is called automatically.
    // generateGeometry() will be called by setupMesh().
}

void Cube::generateGeometry() {
    vertices_data.clear(); // Clear any previous data
    indices_data.clear();

    float hw = width / 2.0f;
    float hh = height / 2.0f;
    float hd = depth / 2.0f;

    // Define unique vertex positions
    glm::vec3 v_ftl = { -hw,  hh,  hd }; // Front-Top-Left
    glm::vec3 v_ftr = { hw,  hh,  hd }; // Front-Top-Right
    glm::vec3 v_fbl = { -hw, -hh,  hd }; // Front-Bottom-Left
    glm::vec3 v_fbr = { hw, -hh,  hd }; // Front-Bottom-Right
    glm::vec3 v_btl = { -hw,  hh, -hd }; // Back-Top-Left
    glm::vec3 v_btr = { hw,  hh, -hd }; // Back-Top-Right
    glm::vec3 v_bbl = { -hw, -hh, -hd }; // Back-Bottom-Left
    glm::vec3 v_bbr = { hw, -hh, -hd }; // Back-Bottom-Right

    // Define normals for each face
    glm::vec3 n_front = { 0.0f, 0.0f, 1.0f };
    glm::vec3 n_back = { 0.0f, 0.0f, -1.0f };
    glm::vec3 n_top = { 0.0f, 1.0f, 0.0f };
    glm::vec3 n_bottom = { 0.0f, -1.0f, 0.0f };
    glm::vec3 n_right = { 1.0f, 0.0f, 0.0f };
    glm::vec3 n_left = { -1.0f, 0.0f, 0.0f };

    // Define standard texture coordinates for a face
    glm::vec2 tc_00 = { 0.0f, 0.0f };
    glm::vec2 tc_10 = { 1.0f, 0.0f };
    glm::vec2 tc_11 = { 1.0f, 1.0f };
    glm::vec2 tc_01 = { 0.0f, 1.0f };

    GLuint current_idx = 0;

    // Front face
    addVertex(v_fbl, faceColor, tc_00, n_front); addVertex(v_fbr, faceColor, tc_10, n_front);
    addVertex(v_ftr, faceColor, tc_11, n_front); addVertex(v_ftl, faceColor, tc_01, n_front);
    indices_data.insert(indices_data.end(), { current_idx, current_idx + 1, current_idx + 2, current_idx, current_idx + 2, current_idx + 3 });
    current_idx += 4;

    // Back face
    addVertex(v_bbr, faceColor, tc_00, n_back); addVertex(v_bbl, faceColor, tc_10, n_back);
    addVertex(v_btl, faceColor, tc_11, n_back); addVertex(v_btr, faceColor, tc_01, n_back);
    indices_data.insert(indices_data.end(), { current_idx, current_idx + 1, current_idx + 2, current_idx, current_idx + 2, current_idx + 3 });
    current_idx += 4;

    // Top face
    addVertex(v_ftl, faceColor, tc_00, n_top); addVertex(v_ftr, faceColor, tc_10, n_top);
    addVertex(v_btr, faceColor, tc_11, n_top); addVertex(v_btl, faceColor, tc_01, n_top);
    indices_data.insert(indices_data.end(), { current_idx, current_idx + 1, current_idx + 2, current_idx, current_idx + 2, current_idx + 3 });
    current_idx += 4;

    // Bottom face
    addVertex(v_bbl, faceColor, tc_00, n_bottom); addVertex(v_bbr, faceColor, tc_10, n_bottom);
    addVertex(v_fbr, faceColor, tc_11, n_bottom); addVertex(v_fbl, faceColor, tc_01, n_bottom);
    indices_data.insert(indices_data.end(), { current_idx, current_idx + 1, current_idx + 2, current_idx, current_idx + 2, current_idx + 3 });
    current_idx += 4;

    // Right face
    addVertex(v_fbr, faceColor, tc_00, n_right); addVertex(v_bbr, faceColor, tc_10, n_right);
    addVertex(v_btr, faceColor, tc_11, n_right); addVertex(v_ftr, faceColor, tc_01, n_right);
    indices_data.insert(indices_data.end(), { current_idx, current_idx + 1, current_idx + 2, current_idx, current_idx + 2, current_idx + 3 });
    current_idx += 4;

    // Left face
    addVertex(v_bbl, faceColor, tc_00, n_left); addVertex(v_fbl, faceColor, tc_10, n_left);
    addVertex(v_ftl, faceColor, tc_11, n_left); addVertex(v_btl, faceColor, tc_01, n_left);
    indices_data.insert(indices_data.end(), { current_idx, current_idx + 1, current_idx + 2, current_idx, current_idx + 2, current_idx + 3 });
    // current_idx += 4; // Not needed for the last face
}