# Build Fixes Summary

## Problemy naprawione

### 1. **Case-sensitivity w includes (Linux)**
Problem: Na Linuxie pliki systemów plików są case-sensitive, ale kod szukał nagłówków z wielkimi literami (np. `Shape.h`), podczas gdy rzeczywiste pliki miały małe litery (np. `shape.h`).

Naprawione pliki:
- `src/main.cpp` - zmieniono: `Shape.h` → `shape.h`, `Cube.h` → `cube.h`, `Plane.h` → `plane.h`, `Pyramid.h` → `pyramid.h`, `Sphere.h` → `sphere.h`, `Cylinder.h` → `cylinder.h`
- `src/shape.cpp` - zmieniono: `Shape.h` → `shape.h`
- `src/cylinder.cpp` - zmieniono: `Cylinder.h` → `cylinder.h`
- `src/pyramid.cpp` - zmieniono: `Pyramid.h` → `pyramid.h`
- `src/sphere.cpp` - zmieniono: `Sphere.h` → `sphere.h`
- `include/TrapezoidPrism.h` - zmieniono: `Shape.h` → `shape.h`
- `include/cylinder.h` - zmieniono: `Shape.h` → `shape.h`
- `include/pyramid.h` - zmieniono: `Shape.h` → `shape.h`
- `include/sphere.h` - zmieniono: `Shape.h` → `shape.h`
- `include/plane.h` - zmieniono: `Shape.h` → `shape.h`

### 2. **Brakujące ścieżki do stb_image.h**
Problem: Kod szukał `<stb/stb_image.h>`, ale plik jest bezpośrednio w `include/`.

Naprawione pliki:
- `include/texture.h` - zmieniono: `#include <stb/stb_image.h>` → `#include "stb_image.h"`
- `src/stb.cpp` - zmieniono: `#include<stb/stb_image.h>` → `#include "stb_image.h"`

### 3. **Windows.h w camera.cpp (niezgodne z Linuxem)**
Problem: `src/camera.cpp` zawierał `#include <Windows.h>` oraz funkcje specyficzne dla Windows w metodzie `printData()`.

Naprawione:
- Usunięto `#include <Windows.h>` z `src/camera.cpp`
- Zastąpiono Windows-specific kod w `Camera::printData()` (GetStdHandle, SetConsoleCursorPosition, itp.) cross-platform wersją używającą tylko `std::cout`

## Wynik

✅ **Projekt buduje się pomyślnie!**

```
[100%] Built target ArtGallery
```

Executable: `build/bin/ArtGallery` (203 KB)
Assets: Skopiowane do `build/bin/assets/` (tekstury i shadery)

## Instrukcje budowy na przyszłość

```bash
cd projekt_grafika_final
rm -rf build && mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j$(nproc)
# Executable: ./bin/ArtGallery
```
