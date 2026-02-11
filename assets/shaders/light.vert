#version 330 core
// Input vertex position attribute.
layout (location = 0) in vec3 aPos;

// Model matrix uniform.
uniform mat4 model;

// Camera matrix uniform.
uniform mat4 camMatrix;

void main()
{
    // Calculate the final vertex position.
    gl_Position = camMatrix * model * vec4(aPos, 1.0f);
}