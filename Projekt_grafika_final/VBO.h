    #ifndef VBO_CLASS_H
    #define VBO_CLASS_H

    #include <glad/glad.h>

    class VBO
    {
    public:
        GLuint ID; // ID of the Vertex Buffer Object

        // Constructor that generates the VBO
        VBO(GLfloat* vertices, GLsizeiptr size);

        // Binds the VBO
        void Bind();

        // Unbinds the VBO
        void Unbind();

        // Deletes the VBO
        void Delete();
    };

    #endif