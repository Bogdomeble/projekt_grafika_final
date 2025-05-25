#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor; // Nadal przekazywane, ale nieu ywane w nowym frag shaderze
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNormal; // Nowe wej cie normalnych

out vec3 color; // Nadal przekazywane
out vec2 texCoord;
out vec3 Normal; // Wyj cie normalnych do fragment shadera
out vec3 crntPos; // Wyj cie pozycji w przestrzeni  wiata

uniform mat4 camMatrix; // Po  czona macierz widoku * projekcji
uniform mat4 model;     // Macierz modelu dla tego obiektu

void main()
{
    // Oblicz pozycj  wierzcho ka w przestrzeni  wiata
    crntPos = vec3(model * vec4(aPos, 1.0f));
    // Przekszta   do przestrzeni obcinania
    gl_Position = camMatrix * vec4(crntPos, 1.0f);

    // Przeka  dane do fragment shadera
    color = aColor; // Nadal przekazywane
    texCoord = aTex;
    // Normalne te  powinny by  transformowane (zwykle przez macierz normalnych),
    // ale dla uproszczenia laboratorium przekazuje je bezpo rednio.
    // W a ciwie: Normal = mat3(transpose(inverse(model))) * aNormal;
    Normal = aNormal; // Uproszczenie z laboratorium
}
