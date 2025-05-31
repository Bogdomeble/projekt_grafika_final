    #ifndef PYRAMID_H
    #define PYRAMID_H

    #include "Shape.h"

    class Pyramid : public Shape {
    private:
        // Specific parameters for the pyramid, e.g., base color and peak color
        glm::vec3 baseColor;
        glm::vec3 peakColor;

    protected:
        // Override the generateGeometry method to define the pyramid's geometry
        void generateGeometry() override;

    public:
        // Constructor for the Pyramid class
        Pyramid(const glm::vec3& bColor = glm::vec3(0.83f, 0.70f, 0.44f),
            const glm::vec3& pColor = glm::vec3(0.92f, 0.86f, 0.76f));
    };

    #endif // PYRAMID_H