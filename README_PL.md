Projekt: OpenGL 3D Scene Renderer (Galeria Sztuki)

Podsumowanie
Krótka, czytelna aplikacja C++ wykorzystująca OpenGL do renderowania prostej sceny 3D (galerii). Zawiera:
- Abstrakcję siatek (klasa bazowa `Shape` + `Cube`, `Plane`, `Pyramid`, `Sphere`, `Cylinder`)
- Shadery GLSL (vertex + fragment) z oświetleniem
- Textury ładowane przez `stb_image`
- Kamerę z ruchem typu FPS i obsługą myszy

Jak to działa (w skrócie)
1. Inicjalizacja
   - Inicjalizacja GLFW, ustawienie okna i callbacków
   - Załadowanie funkcji OpenGL (GLAD) lub inny loader
   - Kompilacja shaderów i ładowanie tekstur

2. Przygotowanie sceny
   - Utworzenie obiektów `Shape`, ustawienie `model` matrix i przypisanie tekstur
   - Wywołanie `setupMesh()` dla każdej siatki (VAO/VBO/EBO)
   - Konfiguracja źródeł światła i kamery

3. Pętla renderująca
   - Obliczenie delta time i obsługa wejścia kamery
   - Czyszczenie buforów, ustawienie uniformów (macierz kamery, parametry światła)
   - Dla każdego obiektu: aktywacja shadera, ustawienie macierzy modelu, bindowanie tekstury, rysowanie `glDrawElements`
   - Swap buffers i poll events

Struktura projektu (zwięźle)
- `src/`      — pliki źródłowe (`main.cpp`, implementacje shape, `shader`, `texture`, `camera`, `VAO/VBO/EBO`)
- `include/`  — nagłówki
- `assets/`   — `shaders/` i `textures/`
- `CMakeLists.txt` lub budowa bezpośrednio przez `g++`

Wymagania
- Kompilator C++17 (GCC/Clang/MSVC)
- Biblioteki: GLFW, GLM, OpenGL development headers
- GLAD lub inny loader (upewnij się, że `glad.c` jest skompilowany)
- `stb_image.h` w include path

Budowanie i uruchamianie (szybkie)
Zalecane: CMake
- mkdir build && cd build
- cmake ..
- cmake --build . --config Release
- Skopiuj katalog `assets/` obok pliku wykonywalnego lub zapewnij poprawne ścieżki
- Uruchom: `./gallery` (Windows: `gallery.exe`)

Minimalny przykład g++ (Linux)
g++ src/*.cpp -Iinclude -o gallery -lglfw -lGL -ldl -pthread
(Upewnij się, że `glad.c` jest kompilowany lub loader jest podlinkowany)

Znalezione/znane problemy (krótko)
- Błędne ścieżki do assetów: brak shaderów/tekstur powoduje błąd lub czarne obiekty — sprawdź ścieżki względem katalogu wykonywalnego.
- Case-sensitivity: na Linuxie wielkość liter w include/ścieżkach i nazwach plików ma znaczenie.
- Normale i skalowanie: przy niejednostajnym skalowaniu modelu trzeba transformować normalne macierzą normalną (transpose(inverse(model))) — inaczej oświetlenie będzie błędne.
- Kamera: hardcodowane wartości `sensitivity` lub `deltaTime` mogą powodować skoki lub zbyt dużą czułość; użyj dynamicznego delta time i ogranicz wartości skrajne.
- Błędy shaderów: kompilacja/linkowanie shaderów może się nie powieść — sprawdzaj logi kompilacji shaderów w konsoli.
- Platformowe includes: usuń zależności specyficzne dla Windows (`Windows.h`) dla buildów cross-platform.

Szybkie wskazówki debugowania
- Sprawdź logi kompilacji shaderów (komunikaty z `glGetShaderInfoLog` / `glGetProgramInfoLog`).
- Weryfikuj obecność plików w `assets/` i ich ścieżki względem wykonywalnego.
- Dodaj proste logowanie przy ładowaniu plików (shadery, tekstury).
- Użyj `glGetError()` dla sprawdzenia błędów OpenGL i waliduj rozmiary VBO/EBO oraz liczbę indeksów.

Współpraca / wkład
Poprawki dotyczące ścieżek, obsługi błędów i dokumentacji są mile widziane. Dokumentuj zmiany dotyczące zależności i struktury assetów.
