#version 330 core
out vec4 FragColor;

in vec3 color; // Nieu ywane w tym shaderze o wietlenia (kolor pochodzi z tekstury i  wiat a)
in vec2 texCoord;
in vec3 Normal;   // Zinterpolowana normalna z vertex shadera
in vec3 crntPos;  // Zinterpolowana pozycja fragmentu w przestrzeni  wiata

uniform sampler2D tex0; // Tekstura obiektu
uniform vec4 lightColor; // Kolor  wiat a
uniform vec3 lightPos;   // Pozycja  wiat a w przestrzeni  wiata
uniform vec3 camPos;     // Pozycja kamery w przestrzeni  wiata

void main()
{
    // O wietlenie otoczenia (ambient)
    float ambient = 0.10f;

    // O wietlenie rozproszone (diffuse)
    vec3 normal = normalize(Normal); // Normalizuj normaln  fragmentu
    vec3 lightDirection = normalize(lightPos - crntPos); // Kierunek od fragmentu do  wiat a
    float diffuse = max(dot(normal, lightDirection), 0.20f); // Intensywno   diffuse

    // O wietlenie odbite (specular)
    float specularLight = 0.20f; // Intensywno    wiat a specular
    vec3 viewDirection = normalize(camPos - crntPos); // Kierunek od fragmentu do kamery
    vec3 reflectionDirection = reflect(-lightDirection, normal); // Kierunek odbicia  wiat a
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8); // Wsp czynnik specular (8 = po yskliwo  )
    float specular = specAmount * specularLight * 0.5f ; // Finalna intensywno   specular

    // Po  cz wszystko: kolor tekstury * kolor  wiat a * (suma komponent w o wietlenia)
    FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}