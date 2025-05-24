#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include <glm/gtc/constants.hpp> // Dla glm::pi()

class Cylinder : public Shape {
private:
    float baseRadius;
    float topRadius; // Mo�na zrobi� sto�ek, je�li topRadius != baseRadius
    float height;
    unsigned int sectorCount; // Liczba segment�w wok� osi
    unsigned int stackCount;  // Liczba segment�w wzd�u� wysoko�ci (dla �cian bocznych)
    bool smoothShading; // Czy cieniowanie �cian bocznych ma by� g�adkie
    glm::vec3 cylinderColor;

protected:
    void generateGeometry() override;
    void buildCap(bool isTop); // Metoda pomocnicza do budowy podstaw

public:
    Cylinder(float br, float tr, float h, unsigned int sectors, unsigned int stacks = 1, bool smooth = true, const glm::vec3& color = glm::vec3(1.0f));
};

#endif // CYLINDER_H