#ifndef PYRAMID_H
#define PYRAMID_H

#include "Shape.h"

class Pyramid : public Shape {
private:
    // Add any specific parameters if needed, e.g., baseSize, height
    // Or use fixed colors as in your original example
    glm::vec3 baseColor;
    glm::vec3 peakColor;

protected:
    void generateGeometry() override;

public:
    Pyramid(const glm::vec3& bColor = glm::vec3(0.83f, 0.70f, 0.44f),
        const glm::vec3& pColor = glm::vec3(0.92f, 0.86f, 0.76f));
};

#endif // PYRAMID_H