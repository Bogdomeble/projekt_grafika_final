#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include <glm/gtc/constants.hpp> // Dla glm::pi()

class Sphere : public Shape {
private:
    float radius;
    unsigned int sectorCount; // Liczba segment�w wok� osi Y
    unsigned int stackCount;  // Liczba segment�w wzd�u� osi Y
    glm::vec3 sphereColor;

protected:
    void generateGeometry() override;

public:
    Sphere(float r, unsigned int sectors, unsigned int stacks, const glm::vec3& color = glm::vec3(1.0f));
};

#endif // SPHERE_H