#include "Sphere.h"
#include <cmath> // Dla sin, cos

Sphere::Sphere(float r, unsigned int sectors, unsigned int stacks, const glm::vec3& color)
    : radius(r), sectorCount(sectors), stackCount(stacks), sphereColor(color) {
    // Konstruktor Shape zostanie wywo�any automatycznie.
    // setupMesh() wywo�a generateGeometry()
}

void Sphere::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    float x, y, z, xy;                              // Pozycja wierzcho�ka
    float nx, ny, nz, lengthInv = 1.0f / radius;    // Normalna wierzcho�ka
    float s, t;                                     // Wsp�rz�dne tekstury

    float sectorStep = 2 * glm::pi<float>() / sectorCount;
    float stackStep = glm::pi<float>() / stackCount;
    float sectorAngle, stackAngle;

    for (unsigned int i = 0; i <= stackCount; ++i) {
        stackAngle = glm::pi<float>() / 2.0f - i * stackStep; // Od pi/2 do -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)  (zamienione y z z dla orientacji Y-up)

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            sectorAngle = j * sectorStep;           // Od 0 do 2pi

            // Pozycja wierzcho�ka (x, y, z) - Y jest osi� pionow�
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            // z jest ju� obliczone

            // Znormalizowana normalna wierzcho�ka (nx, ny, nz)
            nx = x * lengthInv;
            ny = y * lengthInv;
            nz = z * lengthInv;

            // Wsp�rz�dne tekstury (s, t) w zakresie [0, 1]
            s = (float)j / sectorCount;
            t = (float)i / stackCount;

            addVertex(glm::vec3(x, z, y), sphereColor, glm::vec2(s, t), glm::vec3(nx, nz, ny)); // Zamiana y i z dla pozycji i normalnych
			// IMPORTANT: Upewnij si�, �e normalne s� w odpowiedniej orientacji (Y-up)
        }
    }

    // Indeksy
    unsigned int k1, k2;
    for (unsigned int i = 0; i < stackCount; ++i) {
        k1 = i * (sectorCount + 1);     // Pocz�tek bie��cego "stack"
        k2 = k1 + sectorCount + 1;      // Pocz�tek nast�pnego "stack"

        for (unsigned int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
            // 2 tr�jk�ty na sektor (z wyj�tkiem pierwszego i ostatniego "stack")
            if (i != 0) {
                indices_data.push_back(k1);
                indices_data.push_back(k2);
                indices_data.push_back(k1 + 1);
            }

            if (i != (stackCount - 1)) {
                indices_data.push_back(k1 + 1);
                indices_data.push_back(k2);
                indices_data.push_back(k2 + 1);
            }
        }
    }
}