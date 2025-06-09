#include "TrapezoidPrism.h"

TrapezoidPrism::TrapezoidPrism(float width, float height, float depthTop, float depthBottom, const glm::vec3& color)
    : m_width(width), m_height(height), m_depthTop(depthTop), m_depthBottom(depthBottom), m_color(color) {
}

void TrapezoidPrism::generateGeometry() {
    vertices_data.clear();
    indices_data.clear();

    float hw = m_width / 2.0f;
    float ht = m_height;
    float dt = m_depthTop / 2.0f;
    float db = m_depthBottom / 2.0f;

    // Przesuwamy ca?? bry?? tak, by ty? by? w z=0
    float przesuniecie = db;

    // 8 vertices: bottom-front/back, top-front/back
    glm::vec3 v0(-hw, 0, 0);           // bottom left back (przy ?cianie)
    glm::vec3 v1(hw, 0, 0);            // bottom right back (przy ?cianie)
    glm::vec3 v2(hw, 0, 2*db);         // bottom right front (przy pod?odze, na sal?)
    glm::vec3 v3(-hw, 0, 2*db);        // bottom left front

    glm::vec3 v4(-hw, ht, db-dt);      // top left back
    glm::vec3 v5(hw, ht, db-dt);       // top right back
    glm::vec3 v6(hw, ht, db+dt);       // top right front
    glm::vec3 v7(-hw, ht, db+dt);      // top left front

    // Add vertices (positions, color, texcoords, normals)
    // ... (implement as in Cube, with correct indices and normals)
    // For brevity, you can copy the Cube logic and adjust for trapezoid

    // Example: front face (trapezoid)
    addVertex(v3, m_color, { 0,0 }, { 0,0,1 });
    addVertex(v2, m_color, { 1,0 }, { 0,0,1 });
    addVertex(v6, m_color, { 1,1 }, { 0,0,1 });
    addVertex(v7, m_color, { 0,1 }, { 0,0,1 });
    indices_data.insert(indices_data.end(), { 0,1,2, 0,2,3 });
    // ... repeat for other faces
}