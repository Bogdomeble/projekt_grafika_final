    #include "VBO.h"

    // Constructor that generates the Vertex Buffer Object (VBO)
    VBO::VBO(GLfloat* vertices, GLsizeiptr size)
    {
        glGenBuffers(1, &ID); // Generate buffer ID
        glBindBuffer(GL_ARRAY_BUFFER, ID); // Bind the buffer as an array buffer
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW); // Upload vertex data to the buffer
    }

    // Bind the VBO
    void VBO::Bind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, ID);
    }

    // Unbind the VBO
    void VBO::Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    // Delete the VBO
    void VBO::Delete()
    {
        glDeleteBuffers(1, &ID);
    }