# 🎮 Naprawa Czułości Kamery

## Co było nie tak

Kamera była **obłędnie czuła** - nawet najmniejszy ruch myszy obracał widok do 90 stopni.

### Przyczyny:

1. **sensitivity = 100.0f** ❌
   - To jest absurdnie wysoka wartość
   - Każdy pixel ruchu myszy = 100 * pixel/screen_dimension radianów
   - Nawet 1 pixel ruchu = niemal 360 stopni obrotu

2. **Speed pomnożony przez deltaTime dwa razy** ❌
   - `speed = 0.1f * deltaTime` w field
   - Potem `Position += speed * Orientation * deltaTime`
   - To powodowało że speed był za mały i nieprawidłowy

3. **Hardcoded deltaTime = 0.10f** ❌
   - Zamiast rzeczywistego delta time
   - Powodował nierównomierne ruchy

## Rozwiązanie

### Zmienione wartości w camera.h:
```cpp
// Przed:
float speed = 1.0f;
float sensitivity = 100.0f;
float deltaTime = glfwGetTime();

// Po:
float speed = 3.0f;
float sensitivity = 0.1f;
float deltaTime = 0.0f;
```

### Zmieniony Inputs() w camera.cpp:

**Przed:**
```cpp
float deltaTime = 0.10f;  // Hardcoded!
if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    speed = 0.4f * deltaTime;  // Double multiplication!
else
    speed = 0.1f * deltaTime;
```

**Po:**
```cpp
// Calculate actual deltaTime
static float lastTime = glfwGetTime();
float currentTime = glfwGetTime();
deltaTime = currentTime - lastTime;
lastTime = currentTime;

if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    speed = 10.0f;  // Simple value
else
    speed = 3.0f;   // Simple value
```

## Nowe Wartości

| Parametr | Przed | Po | Opis |
|----------|-------|-----|------|
| **sensitivity** | 100.0 | 0.1 | 1000x niższa czułość myszy |
| **speed** | 1.0 | 3.0 | Normalna szybkość poruszania |
| **speed (shift)** | - | 10.0 | Przyspieszenie ze Shift |
| **deltaTime** | 0.10 | dynamic | Rzeczywisty delta time |

## Kontrola

- **WASD** - poruszanie się (szybkość: 3.0)
- **SPACE** - lot w górę
- **CTRL** - lot w dół
- **SHIFT** - przyspieszenie (szybkość: 10.0)
- **LPM** + ruch myszy - obracanie kamery (czułość: 0.1)
- **ESC** - wyjście

## Testowanie

```bash
cd build/bin
./ArtGallery
```

Teraz kamera powinna być dużo bardziej kontrolowalna! 🎯

Jeśli nadal jest za czuła lub za mało czuła, możesz zmienić w `include/camera.h`:
```cpp
float sensitivity = 0.1f;  // Zwiększ dla większej czułości, zmniejsz dla mniejszej
```
