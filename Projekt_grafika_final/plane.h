#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape {
private:
    float p_width;    // Width of the plane
    float p_length;   // Length of the plane
    glm::vec3 p_color; // Color of the plane
    glm::vec2 p_texScale; // Texture scaling factor
    float p_yOffset;  // Y-offset for the plane

protected:
    void generateGeometry() override; // Generates the plane's geometry

public:
    Plane(float width, float length, const glm::vec3& color,
          const glm::vec2& texScale = glm::vec2(1.0f, 1.0f), float yOffset = 0.0f);
};

#endif
