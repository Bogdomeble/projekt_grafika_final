//default.vert

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // Still passed, but unused
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal; // Normal input

out vec3 color;     // Still passed
out vec2 texCoord;
out vec3 Normal;    // Normal output to fragment shader
out vec3 crntPos;   // World space position output

uniform mat4 camMatrix; // Combined view * projection matrix
uniform mat4 model;     // Model matrix for this object

void main()
{
    // Calculate the vertex position in world space
    crntPos = vec3(model * vec4(aPos, 1.0f));
    // Transform to clip space
    gl_Position = camMatrix * vec4(crntPos, 1.0f);

    // Pass data to the fragment shader
    color = aColor;
    texCoord = aTex;

    // Correct normal transformation
    // Use mat3(transpose(inverse(model))) to transform normals.
    // This is important if the model is scaled non-uniformly.
    Normal = mat3(model) * aNormal;
}