#include "Sphere.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Sphere::Sphere(float r, unsigned int sectors, unsigned int stacks, const glm::vec3& color)
    : radius(r), sectorCount(sectors), stackCount(stacks), sphereColor(color) {
    Type = ShapeType::SHAPE_TYPE_SPHERE;
}

void Sphere::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    float x_pos, y_height, z_depth;
    float nx, ny, nz, lengthInv = 1.0f / radius;
    float s, t;
    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    // Generate vertices
    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = glm::pi<float>() / 2.0f - i * stackStep; // from pi/2 to -pi/2
        float xy_projection = radius * cosf(stackAngle);
        y_height = radius * sinf(stackAngle);
        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep; // from 0 to 2pi
            x_pos = xy_projection * cosf(sectorAngle);
            z_depth = xy_projection * sinf(sectorAngle);
            nx = x_pos * lengthInv;
            ny = y_height * lengthInv;
            nz = z_depth * lengthInv;

            // Texture coordinates
            s = 1.0f - ((float)j / sectorCount);
            t = 1.0f - ((float)i / stackCount);

            // Add vertex (position, color, tex coords, normal)
            // The multiplication by glm::vec3(1.0f, 1.0f, 0.0f) is likely a bug and should be removed
            addVertex(glm::vec3(x_pos, z_depth, y_height), sphereColor, glm::vec2(s, t), glm::vec3(nx, nz, ny));
        }
    }

    // Generate indices for triangles
    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);
        k2 = k1 + sectorCount + 1;
        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            if (i != 0) {
                // First triangle of quad
                indices_data.push_back(k1);
                indices_data.push_back(k2);
                indices_data.push_back(k1 + 1);
            }
            if (i != (stackCount - 1)) {
                // Second triangle of quad
                indices_data.push_back(k1 + 1);
                indices_data.push_back(k2);
                indices_data.push_back(k2 + 1);
            }
        }
    }
}