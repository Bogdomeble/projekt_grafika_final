#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // Nadal przekazywane, ale nieu¿ywane w nowym frag shaderze
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal; // Nowe wejœcie normalnych

out vec3 color; // Nadal przekazywane
out vec2 texCoord;
out vec3 Normal; // Wyjœcie normalnych do fragment shadera
out vec3 crntPos; // Wyjœcie pozycji w przestrzeni œwiata

uniform mat4 camMatrix; // Po³¹czona macierz widoku * projekcji
uniform mat4 model;     // Macierz modelu dla tego obiektu

void main()
{
    // Oblicz pozycjê wierzcho³ka w przestrzeni œwiata
    crntPos = vec3(model * vec4(aPos, 1.0f));
    // Przekszta³æ do przestrzeni obcinania
    gl_Position = camMatrix * vec4(crntPos, 1.0f);

    // Przeka¿ dane do fragment shadera
    color = aColor; // Nadal przekazywane
    texCoord = aTex;
    // Normalne te¿ powinny byæ transformowane (zwykle przez macierz normalnych),
    // ale dla uproszczenia laboratorium przekazuje je bezpoœrednio.
    // W³aœciwie: Normal = mat3(transpose(inverse(model))) * aNormal;
    Normal = aNormal; // Uproszczenie z laboratorium
}