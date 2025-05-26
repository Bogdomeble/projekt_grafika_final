#version 330 core
out vec4 FragColor;

in vec3 color;    // nieużywane w tej wersji światła (możesz je wykorzystać np. do debug’u)
in vec2 texCoord;
in vec3 Normal;   // zinterpolowana normalna
in vec3 crntPos;  // zinterpolowana pozycja fragmentu w przestrzeni świata

uniform sampler2D tex0;   // tekstura obiektu
uniform vec4 lightColor;  // kolor światła ruchomego sufitowego
uniform vec3 lightPos;    // pozycja ruchomego światła (sufit)
uniform vec4 lampColor;   // kolor żarówki lampy
uniform vec3 lampPos;     // pozycja żarówki lampy
uniform vec3 camPos;      // pozycja kamery

void main()
{
    // --- Ambient ---
    float ambientStrength = 0.08;
    vec3 ambient = ambientStrength * vec3(lightColor.rgb + lampColor.rgb);

    // --- Diffuse (światło sufitowe) ---
    vec3 norm = normalize(Normal);
    vec3 lightDir1 = normalize(lightPos - crntPos);
    float diff1 = max(dot(norm, lightDir1), 0.0);

    // --- Diffuse (światło lampy) ---
    vec3 lightDir2 = normalize(lampPos - crntPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);

    // --- Specular (sufitowe) ---
    float specularStrength1 = 0.15;
    vec3 viewDir = normalize(camPos - crntPos);
    vec3 reflectDir1 = reflect(-lightDir1, norm);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 64.0);
    vec3 specular1 = specularStrength1 * spec1 * vec3(lightColor.rgb);

    // --- Specular (lampy) ---
    float specularStrength2 = 0.15;
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 64.0);
    vec3 specular2 = specularStrength2 * spec2 * vec3(lampColor.rgb);

    // --- Połącz w jeden wynik oświetlenia ---
    vec3 diffuse = diff1 * vec3(lightColor.rgb) + diff2 * vec3(lampColor.rgb);
    vec3 specular = specular1 + specular2;
    vec3 lighting = ambient + diffuse + specular;

    // finalny kolor = tekstura * oświetlenie
    vec4 texColor = texture(tex0, texCoord);
    FragColor = vec4(texColor.rgb * lighting, texColor.a);
}
