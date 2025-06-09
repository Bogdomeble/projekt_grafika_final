#pragma once
#include "Shape.h"
#include <glm/glm.hpp>

class TrapezoidPrism : public Shape {
public:
    // width: along the wall, depth: how far it sticks out, height: vertical height
    TrapezoidPrism(float width, float height, float depthTop, float depthBottom, const glm::vec3& color);
    void generateGeometry() override;
private:
    float m_width, m_height, m_depthTop, m_depthBottom;
    glm::vec3 m_color;
};