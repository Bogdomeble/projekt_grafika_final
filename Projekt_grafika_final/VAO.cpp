#include "VAO.h"

// Constructor: generates a new Vertex Array Object (VAO)
VAO::VAO()
{
    glGenVertexArrays(1, &ID);
}

// Links a VBO to this VAO with custom attribute parameters
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents,
    GLenum type, GLsizei stride, void* offset)
{
    VBO.Bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Links a VBO to this VAO using default parameters (3 floats per vertex)
void VAO::LinkVBO(VBO& VBO, GLuint layout)
{
    VBO.Bind();
    glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(layout);
    VBO.Unbind();
}

// Binds this VAO
void VAO::Bind()
{
    glBindVertexArray(ID);
}

// Unbinds any VAO
void VAO::Unbind()
{
    glBindVertexArray(0);
}

// Deletes this VAO
void VAO::Delete()
{
    glDeleteVertexArrays(1, &ID);
}