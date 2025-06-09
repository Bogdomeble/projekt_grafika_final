    #ifndef SHAPE_H
    #define SHAPE_H

    #include <vector>
    #include <string>
    #include <memory> // For std::unique_ptr
    #include <glad/glad.h>
    #include <glm/glm.hpp>
    #include "shaderClass.h" // For passing shader to draw method
    #include "VAO.h"         // Your existing VAO class
    #include "VBO.h"         // Your existing VBO class
    #include "EBO.h"         // Your existing EBO class

    #include "texture.h"


    enum ShapeType {
        SHAPE_TYPE_CUBE,
        SHAPE_TYPE_SPHERE,
        SHAPE_TYPE_PLANE,
	    SHAPE_TYPE_CYLINDER,
	    SHAPE_TYPE_PYRAMID,
	    SHAPE_TYPE_LIGHT,
        SHAPE_TYPE_CUSTOM // For any custom shapes
    };

    class Shape {
    protected:
	    // Type of the shape, useful for identification
        std::vector<GLfloat> vertices_data; // Stores interleaved vertex attributes
        std::vector<GLuint> indices_data;   // Stores vertex indices for EBO

        VAO vao; // Each shape owns its VAO
        std::unique_ptr<VBO> vbo_ptr; // Using unique_ptr for VBO
        std::unique_ptr<EBO> ebo_ptr; // Using unique_ptr for EBO

        bool meshInitialized = false;

        // Helper for derived classes to add a complete vertex's attributes
        void addVertex(const glm::vec3& pos, const glm::vec3& col, const glm::vec2& tex, const glm::vec3& norm);

        // Pure virtual function for derived classes to implement their geometry generation
        virtual void generateGeometry() = 0;

	    Texture* shapeTexture = nullptr; // Optional: Pointer to a texture object if needed

    public:
        ShapeType Type;
        glm::mat4 modelMatrix; // Each shape instance can have its own model matrix
        const size_t stride = 11 * sizeof(GLfloat); // Matches your vertex attribute layout

        Shape();
        virtual ~Shape(); // Important for proper cleanup with polymorphism

        // Initializes VBO, EBO, and configures VAO. Calls generateGeometry if needed.
        virtual void setupMesh();

        // Draws the shape using the provided shader
        virtual void draw(Shader& shader);

        // Cleans up OpenGL resources (VBO, EBO, VAO)
        void cleanup();

        // Accessors (optional, but can be useful for debugging or direct manipulation)
        const std::vector<GLfloat>& getVertices() const { return vertices_data; }
        const std::vector<GLuint>& getIndices() const { return indices_data; }
        GLsizeiptr getVerticesSizeInBytes() const { return vertices_data.size() * sizeof(GLfloat); }
        GLsizeiptr getIndicesSizeInBytes() const { return indices_data.size() * sizeof(GLuint); }
        GLsizei getIndexCount() const { return static_cast<GLsizei>(indices_data.size()); }

        void setTexture(Texture* tex);
    };

    #endif // SHAPE_H