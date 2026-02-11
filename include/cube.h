#ifndef CUBE_H
#define CUBE_H

#include "shape.h"

class Cube : public Shape {
private:
    float width, height, depth;
    glm::vec3 faceColor; // Single color for all faces, or you can get more complex
    
protected:
    void generateGeometry() override; // Implementation of geometry generation

public:
    Cube(float w, float h, float d, const glm::vec3& color);
};

#endif // CUBE_H