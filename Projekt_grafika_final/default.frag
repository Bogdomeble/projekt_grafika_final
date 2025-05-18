#version 330 core
out vec4 FragColor;

in vec3 color; // Nieu¿ywane w tym shaderze oœwietlenia (kolor pochodzi z tekstury i œwiat³a)
in vec2 texCoord;
in vec3 Normal;   // Zinterpolowana normalna z vertex shadera
in vec3 crntPos;  // Zinterpolowana pozycja fragmentu w przestrzeni œwiata

uniform sampler2D tex0; // Tekstura obiektu
uniform vec4 lightColor; // Kolor œwiat³a
uniform vec3 lightPos;   // Pozycja œwiat³a w przestrzeni œwiata
uniform vec3 camPos;     // Pozycja kamery w przestrzeni œwiata

void main()
{
    // Oœwietlenie otoczenia (ambient)
    float ambient = 0.10f;

    // Oœwietlenie rozproszone (diffuse)
    vec3 normal = normalize(Normal); // Normalizuj normaln¹ fragmentu
    vec3 lightDirection = normalize(lightPos - crntPos); // Kierunek od fragmentu do œwiat³a
    float diffuse = max(dot(normal, lightDirection), 0.0f); // Intensywnoœæ diffuse

    // Oœwietlenie odbite (specular)
    float specularLight = 0.80f; // Intensywnoœæ œwiat³a specular
    vec3 viewDirection = normalize(camPos - crntPos); // Kierunek od fragmentu do kamery
    vec3 reflectionDirection = reflect(-lightDirection, normal); // Kierunek odbicia œwiat³a
    float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8); // Wspó³czynnik specular (8 = po³yskliwoœæ)
    float specular = specAmount * specularLight * 0.5f ; // Finalna intensywnoœæ specular

    // Po³¹cz wszystko: kolor tekstury * kolor œwiat³a * (suma komponentów oœwietlenia)
    FragColor = texture(tex0, texCoord) * lightColor * (diffuse + ambient + specular);
}