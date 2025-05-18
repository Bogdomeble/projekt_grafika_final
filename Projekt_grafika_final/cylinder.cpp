#include "Cylinder.h"
#include <cmath>
#include <vector> // Dla std::vector w metodzie pomocniczej

Cylinder::Cylinder(float br, float tr, float h, unsigned int sectors, unsigned int stacks, bool smooth, const glm::vec3& color)
    : baseRadius(br), topRadius(tr), height(h), sectorCount(sectors), stackCount(stacks), smoothShading(smooth), cylinderColor(color) {
    if (stackCount == 0) stackCount = 1; // Co najmniej 1 stack
}

void Cylinder::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    float halfHeight = height / 2.0f;
    float sectorStep = 2.0f * glm::pi<float>() / sectorCount;
    float currentAngle;

    // --- Œciany boczne ---
    GLuint baseSideIndex = 0; // Indeks pocz¹tkowy dla œcian bocznych

    for (unsigned int i = 0; i <= stackCount; ++i) {
        float stackY = -halfHeight + (float)i / stackCount * height;
        float currentRadius = baseRadius + (float)i / stackCount * (topRadius - baseRadius); // Interpolacja promienia

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            currentAngle = j * sectorStep;
            float x = currentRadius * cosf(currentAngle);
            float z = currentRadius * sinf(currentAngle);

            glm::vec3 normal;
            if (smoothShading && (topRadius != 0 || baseRadius != 0)) { // Unikaj dzielenia przez zero, jeœli promieñ to 0
                // G³adkie cieniowanie: normalna prostopad³a do powierzchni bocznej
                // Jeœli to sto¿ek, normalna jest bardziej skomplikowana. Dla cylindra:
                if (topRadius == baseRadius) { // Czysty cylinder
                     normal = glm::normalize(glm::vec3(cosf(currentAngle), 0.0f, sinf(currentAngle)));
                } else { // Sto¿ek lub œciêty sto¿ek - uproszczona normalna (lepsza wymaga³aby wiêcej geometrii)
                    glm::vec3 tangent1 = glm::vec3(-sinf(currentAngle), 0.0f, cosf(currentAngle));
                    glm::vec3 tangent2 = glm::vec3(cosf(currentAngle) * (topRadius-baseRadius)/height, 1.0f, sinf(currentAngle) * (topRadius-baseRadius)/height );
                    normal = glm::normalize(glm::cross(tangent2,tangent1)); // Mo¿e wymagaæ odwrócenia
                }
            } else {
                // P³askie cieniowanie (normalna na face, trudniejsze do zrobienia per vertex bez duplikacji)
                // Dla uproszczenia u¿yjemy promieniowej normalnej, jak w smooth
                 normal = glm::normalize(glm::vec3(cosf(currentAngle), 0.0f, sinf(currentAngle)));
            }
            
            // Wspó³rzêdne tekstury dla œcian bocznych
            float u = (float)j / sectorCount;
            float v = (float)i / stackCount;

            addVertex(glm::vec3(x, stackY, z), cylinderColor, glm::vec2(u, v), normal);
        }
    }

    // Indeksy dla œcian bocznych
    for (unsigned int i = 0; i < stackCount; ++i) {
        GLuint k1 = baseSideIndex + i * (sectorCount + 1); // Pocz¹tek bie¿¹cego "stack"
        GLuint k2 = k1 + (sectorCount + 1);           // Pocz¹tek nastêpnego "stack"

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


    // --- Podstawy (Caps) ---
    // Dolna podstawa
    if (baseRadius > 0.0f) {
        GLuint bottomCenterIndex = baseSideIndex;
        addVertex(glm::vec3(0.0f, -halfHeight, 0.0f), cylinderColor, glm::vec2(0.5f, 0.5f), glm::vec3(0.0f, -1.0f, 0.0f));
        baseSideIndex++;

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            currentAngle = j * sectorStep;
            float x = baseRadius * cosf(currentAngle);
            float z = baseRadius * sinf(currentAngle);
            float u = 0.5f + 0.5f * cosf(currentAngle); // Proste mapowanie planarne dla podstaw
            float v = 0.5f + 0.5f * sinf(currentAngle);
            addVertex(glm::vec3(x, -halfHeight, z), cylinderColor, glm::vec2(u, v), glm::vec3(0.0f, -1.0f, 0.0f));
        }
        // Indeksy dla dolnej podstawy (wachlarz trójk¹tów)
        for (unsigned int j = 0; j < sectorCount; ++j) {
            indices_data.push_back(bottomCenterIndex);
            indices_data.push_back(baseSideIndex + j);
            indices_data.push_back(baseSideIndex + j + 1);
        }
        baseSideIndex += (sectorCount + 1);
    }


    // Górna podstawa
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
        // Indeksy dla górnej podstawy
        for (unsigned int j = 0; j < sectorCount; ++j) {
            indices_data.push_back(topCenterIndex);
            indices_data.push_back(baseSideIndex + j + 1); // Odwrócona kolejnoœæ dla CCW
            indices_data.push_back(baseSideIndex + j);
        }
        // baseSideIndex += (sectorCount + 1); // Niepotrzebne, to ostatnia czêœæ
    }
}