#include "Cylinder.h"
#include <cmath>
#include <vector> // For std::vector in helper method

Cylinder::Cylinder(float baseRadius, float topRadius, float height, unsigned int sectors, unsigned int stacks, bool smooth, const glm::vec3& color)
    : baseRadius(baseRadius), topRadius(topRadius), height(height), sectorCount(sectors), stackCount(stacks), smoothShading(smooth), cylinderColor(color) {
    if (stackCount == 0) stackCount = 1; // At least 1 stack
	if (sectorCount == 0) sectorCount = 36; // At least 36 sectors for a full circle
    Type = ShapeType::SHAPE_TYPE_CYLINDER;
}

void Cylinder::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    float halfHeight = height / 2.0f;
    float sectorStep = 2.0f * glm::pi<float>() / sectorCount;
    float currentAngle;

    // --- Side Walls ---
    GLuint baseSideIndex = 0; // Starting index for side walls

    for (unsigned int i = 0; i <= stackCount; ++i) {
        float stackY = -halfHeight + (float)i / stackCount * height;
        float currentRadius = baseRadius + (float)i / stackCount * (topRadius - baseRadius); // Radius interpolation

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            currentAngle = j * sectorStep;
            float x = currentRadius * cosf(currentAngle);
            float z = currentRadius * sinf(currentAngle);

            glm::vec3 normal;
            if (smoothShading && (topRadius != 0 || baseRadius != 0)) { // Avoid division by zero if radius is 0
                // Smooth shading: normal perpendicular to the side surface
                // If it's a cone, the normal is more complex. For a cylinder:
                if (topRadius == baseRadius) { // Pure cylinder
                     normal = glm::normalize(glm::vec3(cosf(currentAngle), 0.0f, sinf(currentAngle)));
                } else { // Cone or truncated cone - simplified normal (better would require more geometry)
                    glm::vec3 tangent1 = glm::vec3(-sinf(currentAngle), 0.0f, cosf(currentAngle));
                    glm::vec3 tangent2 = glm::vec3(cosf(currentAngle) * (topRadius-baseRadius)/height, 1.0f, sinf(currentAngle) * (topRadius-baseRadius)/height );
                    normal = glm::normalize(glm::cross(tangent2,tangent1)); // May need to be reversed
                }
            } else {
                // Flat shading (normal per face, harder to do per vertex without duplication)
                // For simplicity, use radial normal, as in smooth
                 normal = glm::normalize(glm::vec3(cosf(currentAngle), 0.0f, sinf(currentAngle)));
            }
            
            // Texture coordinates for side walls
            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;

            addVertex(glm::vec3(x, stackY, z), cylinderColor, glm::vec2(u, v), normal);
        }
    }

    // Indices for side walls
    for (unsigned int i = 0; i < stackCount; ++i) {
        GLuint k1 = baseSideIndex + i * (sectorCount + 1); // Start of the current "stack"
        GLuint k2 = k1 + (sectorCount + 1);           // Start of the next "stack"

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            indices_data.push_back(k1);
            indices_data.push_back(k2);
            indices_data.push_back(k1 + 1);

            indices_data.push_back(k2);
            indices_data.push_back(k2 + 1);
            indices_data.push_back(k1 + 1);
        }
    }
    baseSideIndex += (stackCount + 1) * (sectorCount + 1);


    // --- Caps (Bases) ---
    // Bottom cap
    if (baseRadius > 0.0f) {
        GLuint bottomCenterIndex = baseSideIndex;
        addVertex(glm::vec3(0.0f, -halfHeight, 0.0f), cylinderColor, glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f));
        baseSideIndex++;

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            currentAngle = j * sectorStep;
            float x = baseRadius * cosf(currentAngle);
            float z = baseRadius * sinf(currentAngle);
            float u = 0.5f + 0.5f * cosf(currentAngle); // Simple planar mapping for caps
            float v = 0.5f + 0.5f * sinf(currentAngle);
            addVertex(glm::vec3(x, -halfHeight, z), cylinderColor, glm::vec2(u, v), glm::vec3(0.0f, -1.0f, 0.0f));
        }
        // Indices for the bottom cap (triangle fan)
        for (unsigned int j = 0; j < sectorCount; ++j) {
            indices_data.push_back(bottomCenterIndex);
            indices_data.push_back(baseSideIndex + j);
            indices_data.push_back(baseSideIndex + j + 1);
        }
        baseSideIndex += (sectorCount + 1);
    }


    // Top cap
    if (topRadius > 0.0f) {
        GLuint topCenterIndex = baseSideIndex;
        addVertex(glm::vec3(0.0f, halfHeight, 0.0f), cylinderColor, glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        baseSideIndex++;

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            currentAngle = j * sectorStep;
            float x = topRadius * cosf(currentAngle);
            float z = topRadius * sinf(currentAngle);
            float u = 0.5f + 0.5f * cosf(currentAngle);
            float v = 0.5f + 0.5f * sinf(currentAngle);
            addVertex(glm::vec3(x, halfHeight, z), cylinderColor, glm::vec2(u, v), glm::vec3(0.0f, 1.0f, 0.0f));
        }
        // Indices for the top cap
        for (unsigned int j = 0; j < sectorCount; ++j) {
            indices_data.push_back(topCenterIndex);
            indices_data.push_back(baseSideIndex + j + 1); // Reversed order for CCW
            indices_data.push_back(baseSideIndex + j);
        }
        // baseSideIndex += (sectorCount + 1); // Not needed, this is the last part
    }
}