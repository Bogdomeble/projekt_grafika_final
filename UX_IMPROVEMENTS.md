# 🎮 Ulepszenia UX - FPS Counter i Smooth Camera

## Co się zmieniło

### 1. FPS Counter w Tytule Okna ✅
- Tytuł okna aktualizuje się co 0.25 sekundy
- Pokazuje: "Art Gallery - FPS: XXX"
- Pomaga monitorować wydajność aplikacji

**Implementacja:**
```cpp
// W main.cpp render loop:
- Liczymy fps co 0.25 sekund
- Aktualizujemy tytuł za pomocą: glfwSetWindowTitle(window, title.c_str())
```

### 2. Smooth Camera Movement ✅
Naprawiono "janky" kamerę poprzez:

#### a) Frame-Rate Independent Motion
```cpp
// Klawiatura (WASD/SPACE/CTRL)
Position += speed * Direction * deltaTime;  // Zawsze mnożymy przez deltaTime!
```

#### b) Frame-Rate Independent Mouse Rotation
```cpp
// Mysz (LPM + ruch)
float rotX = sensitivity * (mouseY - height/2) / height * deltaTime * 50.0f;
float rotY = sensitivity * (mouseX - width/2) / width * deltaTime * 50.0f;
```

#### c) DeltaTime Clamping
```cpp
// Zapobieganie ogromnym skokom (np. gdy app był zapauzowany)
if (deltaTime > 0.016f) deltaTime = 0.016f;  // Max 60 FPS minimum
if (deltaTime < 0.001f) deltaTime = 0.001f;  // Min 1ms
```

## 🎯 Korzyści

| Problem | Rozwiązanie | Efekt |
|---------|------------|-------|
| Janky camera | Frame-rate independent motion | Smooth ruchy niezależnie od FPS |
| Brak info o wydajności | FPS counter w tytule | Wiesz ile FPS masz |
| Niedeterministyczne skalamie | DeltaTime clamping | Przewidywalne, stabilne ruchy |
| Mogły być duże skokami ruchu | DeltaTime constraints | Maks 16ms per frame |

## 📊 Parametry

```
Sensitivity: 0.5
- Pomnażane przez: deltaTime * 50.0
- Wynik: Smooth, responsywne ruchy

Speed Normal: 3.0 u/s
Speed + SHIFT: 10.0 u/s
- Pomnażane przez: deltaTime
- Wynik: Liniowe, przewidywalne ruchy

DeltaTime Range: [0.001, 0.016]
- Zapobiega skokom i artefaktom
```

## 🧪 Co Testować

1. **FPS Counter**
   - Sprawdź czy tytuł okna pokazuje FPS
   - Powinien się aktualizować co ~0.25s
   - FPS powinno być 100+ (w zależności od GPU)

2. **Camera Smoothness**
   - Używaj WASD - ruchy powinny być płynne
   - Obracaj kamerę (LPM + mysz) - powinno być smooth
   - Szybko ruchaj myszą - kamera nie powinna "teleportować"

3. **Frame Rate Changes**
   - Camera movement powinno być identyczne niezależnie od FPS
   - Przy 60 FPS i 144 FPS - ten sam prędkość kamery

## 📝 Zmienione Pliki

1. `src/main.cpp`
   - Dodano FPS counter
   - Dodano deltaTime calculation
   - Dodano window title update

2. `src/camera.cpp`
   - Dodano deltaTime clamping
   - Poprawiono mouse sensitivity (mnożenie przez deltaTime)
   - Poprawiono movement smoothness

## 🚀 Kompilacja i Test

```bash
cd projekt_grafika_final/build
cmake --build . -j4
cd bin
./ArtGallery
```

Obserwuj tytuł okna - powinno pokazywać FPS! 📊

## 💡 Jeśli Kamera Nadal Jest Janky

Możliwe przyczyny:
1. Wysoki sensitivity - zmniejsz w `include/camera.h`
2. Niski FPS - sprawdź czy GPU wystarczy
3. Cpu bottleneck - zmniejsz geometrię sceny

Możesz też dostrajać:
- `sensitivity` w camera.h (teraz 0.5)
- `speed` w camera.h (teraz 3.0)
- DeltaTime clamp (teraz 0.016)

