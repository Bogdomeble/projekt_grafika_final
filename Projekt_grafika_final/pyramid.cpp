#include "Pyramid.h"

Pyramid::Pyramid(const glm::vec3& bColor, const glm::vec3& pColor) : baseColor(bColor), peakColor(pColor) {}

void Pyramid::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    // Adapted from your original vertices and indices
    // COORDINATES / COLORS / TexCoord / NORMALS
    // Bottom Face
    addVertex({ -0.5f, 0.0f,  0.5f }, baseColor, { 0.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }); // 0
    addVertex({ -0.5f, 0.0f, -0.5f }, baseColor, { 0.0f, 5.0f }, { 0.0f, -1.0f, 0.0f }); // 1
    addVertex({ 0.5f, 0.0f, -0.5f }, baseColor, { 5.0f, 5.0f }, { 0.0f, -1.0f, 0.0f }); // 2
    addVertex({ 0.5f, 0.0f,  0.5f }, baseColor, { 5.0f, 0.0f }, { 0.0f, -1.0f, 0.0f }); // 3

    // Left Side
    addVertex({ -0.5f, 0.0f,  0.5f }, baseColor, { 0.0f, 0.0f }, { -0.8f, 0.5f,  0.0f }); // 4
    addVertex({ -0.5f, 0.0f, -0.5f }, baseColor, { 5.0f, 0.0f }, { -0.8f, 0.5f,  0.0f }); // 5
    addVertex({ 0.0f, 0.8f,  0.0f }, peakColor, { 2.5f, 5.0f }, { -0.8f, 0.5f,  0.0f }); // 6

    // Back Side
    addVertex({ -0.5f, 0.0f, -0.5f }, baseColor, { 5.0f, 0.0f }, { 0.0f, 0.5f, -0.8f }); // 7
    addVertex({ 0.5f, 0.0f, -0.5f }, baseColor, { 0.0f, 0.0f }, { 0.0f, 0.5f, -0.8f }); // 8
    addVertex({ 0.0f, 0.8f,  0.0f }, peakColor, { 2.5f, 5.0f }, { 0.0f, 0.5f, -0.8f }); // 9

    // Right Side
    addVertex({ 0.5f, 0.0f, -0.5f }, baseColor, { 0.0f, 0.0f }, { 0.8f, 0.5f,  0.0f }); // 10
    addVertex({ 0.5f, 0.0f,  0.5f }, baseColor, { 5.0f, 0.0f }, { 0.8f, 0.5f,  0.0f }); // 11
    addVertex({ 0.0f, 0.8f,  0.0f }, peakColor, { 2.5f, 5.0f }, { 0.8f, 0.5f,  0.0f }); // 12

    // Front Side
    addVertex({ 0.5f, 0.0f,  0.5f }, baseColor, { 5.0f, 0.0f }, { 0.0f, 0.5f,  0.8f }); // 13
    addVertex({ -0.5f, 0.0f,  0.5f }, baseColor, { 0.0f, 0.0f }, { 0.0f, 0.5f,  0.8f }); // 14
    addVertex({ 0.0f, 0.8f,  0.0f }, peakColor, { 2.5f, 5.0f }, { 0.0f, 0.5f,  0.8f }); // 15

    indices_data = {
        0, 1, 2, // Bottom side
        0, 2, 3, // Bottom side
        4, 6, 5, // Left side
        7, 9, 8, // Back side
        10, 12, 11, // Right side
        13, 15, 14 // Front side
    };
}