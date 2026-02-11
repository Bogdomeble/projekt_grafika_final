#version 330 core
out vec4 FragColor;

in vec3 color;         // Unused (color from texture and light)
in vec2 texCoord;
in vec3 Normal;        // Interpolated normal from vertex shader
in vec3 crntPos;       // Interpolated fragment position in world space

struct PointLight {
    vec3 position;
    vec4 color;
};

#define MAX_POINT_LIGHTS 4
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int numActiveLights;

uniform sampler2D tex0;
uniform vec3 camPos;

void main()
{
    vec3 norm = normalize(Normal);
    vec4 textureColorSample = texture(tex0, texCoord);
    vec3 viewDir = normalize(camPos - crntPos);

    // Ambient lighting
    float ambientStrength = 0.20f;
    vec3 ambient = ambientStrength * vec3(0.63, 0.57, 0.3); // General ambient light

    vec3 totalLightContribution = vec3(0.0);

    // Loop through active point lights
    for (int i = 0; i < numActiveLights; ++i)
    {
        vec3 lightDir = normalize(pointLights[i].position - crntPos);

        // Diffuse lighting
        float diff = max(dot(norm, lightDir), 0.1f);
        vec3 diffuse = diff * pointLights[i].color.rgb;

        // Specular lighting
        float specularStrength = 0.35f; // Specular intensity (adjust as needed)
        float shininess = 32.0f;      // Shininess (higher value = smaller, sharper highlight)
                                        // For very smooth surfaces like metal/glass: 64, 128 or more
                                        // For matte surfaces: 8, 16

        vec3 reflectDir = reflect(-lightDir, norm);
        float specAmount = pow(max(dot(viewDir, reflectDir), 0.1f), shininess);
        vec3 specular = specAmount * specularStrength * pointLights[i].color.rgb;

        totalLightContribution += diffuse + specular;
    }

    vec3 finalColor = (ambient + totalLightContribution) * textureColorSample.rgb;
    FragColor = vec4(finalColor, textureColorSample.rgb);
}