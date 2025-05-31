// VAO.h
#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:
    GLuint ID; // ID of the VAO
    VAO();

    // More flexible attribute pointer setup
    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents,
        GLenum type, GLsizei stride, void* offset);

    // Original method for backward compatibility
    void LinkVBO(VBO& VBO, GLuint layout);

    void Bind();   // Binds the VAO
    void Unbind(); // Unbinds the VAO
    void Delete(); // Deletes the VAO
};
#endif