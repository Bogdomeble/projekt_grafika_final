#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>
#include <memory>
#include "cube.h"

class PointLightData {
public:
    glm::vec3 position; // Position of the light source
    glm::vec4 color;    // Color of the light source
    std::unique_ptr<Cube> visualRepresentation; // Visual representation of the light (a cube)

    explicit PointLightData(glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0), glm::vec4 col = glm::vec4(0.0, 0.0, 0.0, 1.0))
        : position(pos), color(col) {
        // Create a cube to visually represent the light source
        visualRepresentation = std::make_unique<Cube>(0.15f, 0.15f, 0.15f, glm::vec3(col.r, col.g, col.b));
        visualRepresentation->setupMesh(); // Initialize the cube's mesh
    }
};

#endif // LIGHT_H
