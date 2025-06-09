#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include <glm/gtc/constants.hpp> // For glm::pi()

class Cylinder : public Shape {
private:
    float baseRadius;
    float topRadius; // Can create a cone if topRadius != baseRadius
    float height;
    unsigned int sectorCount; // Number of segments around the axis
    unsigned int stackCount;  // Number of segments along the height (for side walls)
    bool smoothShading; // Whether the shading of the side walls should be smooth
    glm::vec3 cylinderColor;

protected:
    void generateGeometry() override;
    void buildCap(bool isTop); // Helper method to build the caps

public:
    Cylinder(float br, float tr, float h, unsigned int sectors, unsigned int stacks = 1, bool smooth = true, const glm::vec3& color = glm::vec3(1.0f));
};

#endif // CYLINDER_H