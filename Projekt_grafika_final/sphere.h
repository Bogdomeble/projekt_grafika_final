    #ifndef SPHERE_H
    #define SPHERE_H

    #include "Shape.h"
    #include <glm/gtc/constants.hpp> // For glm::pi()

    class Sphere : public Shape {
    private:
        float radius;
        unsigned int sectorCount; // Number of segments around the Y axis
        unsigned int stackCount;  // Number of segments along the Y axis
        glm::vec3 sphereColor;

    protected:
        // Generates the geometry for the sphere
        void generateGeometry() override;

    public:
        // Constructor for the Sphere class
        Sphere(float r, unsigned int sectors, unsigned int stacks, const glm::vec3& color = glm::vec3(1.0f));
    };

    #endif // SPHERE_H