#version 330 core

// Output color
out vec4 FragColor;

// Uniform variable for light color
uniform vec4 lightColor;

void main()
{
    // Set the fragment color to the light color
    FragColor = lightColor;
}
