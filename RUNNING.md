# Uruchamianie aplikacji

## Jak uruchomić

```bash
cd build/bin
./ArtGallery
```

## Oczekiwane wyjście

Jeśli program się uruchomia poprawnie, powinieneś zobaczyć:

1. **Komunikaty ładowania shaderów:**
   ```
   Loading vertex shader: assets/shaders/default.vert
   Loading fragment shader: assets/shaders/default.frag
   Shaders loaded and compiled successfully
   Loading vertex shader: assets/shaders/light.vert
   Loading fragment shader: assets/shaders/light.frag
   Shaders loaded and compiled successfully
   ```

2. **Okno OpenGL powinno się otworzyć** z galerie sztuki (1920x1080)

3. **Komunikaty debugowania** na konsoli (pozycja kamery, orientacja itp.)

## Kontrola w aplikacji

- **WASD** - poruszanie się
- **Mouse** - obracanie kamery
- **ESC** - zamknięcie okna

## Jeśli program się nie uruchomia

### Problem: "Failed to open file"
- Upewnij się że uruchamiasz z katalogu `build/bin/`
- Upewnij się że `assets/` folder istnieje w `build/bin/assets/`

### Problem: GLFW/GLAD initialization failed
- Upewnij się że masz zainstalowane zależności OpenGL
- Na Ubuntu/Debian: `sudo apt-get install libgl1-mesa-dev libxrandr-dev libxinerama-dev`

### Problem: Shader compilation failed
- Sprawdź czy pliki shader istnieją w `build/bin/assets/shaders/`
- Sprawdź contentu shaderów - mogą zawierać błędy

## Kompilacja

```bash
cd projekt_grafika_final
rm -rf build && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(nproc)
```
