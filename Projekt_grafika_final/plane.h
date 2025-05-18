#ifndef PLANE_H
#define PLANE_H

#include "Shape.h"

class Plane : public Shape {
private:
    float p_width, p_length;
    glm::vec3 p_color;
    glm::vec2 p_texScale;
    float p_yOffset;

protected:
    void generateGeometry() override;

public:
    Plane(float width, float length, const glm::vec3& color,
        const glm::vec2& texScale = glm::vec2(1.0f, 1.0f), float yOffset = 0.0f);
};
#endif
