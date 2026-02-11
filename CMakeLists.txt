cmake_minimum_required(VERSION 3.15)

project(ArtGalleryRenderer
    VERSION 1.0.0
    DESCRIPTION "OpenGL-based Art Gallery 3D Scene Renderer"
    LANGUAGES C CXX
)

# ============================================================================
# Project Configuration
# ============================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ============================================================================
# Compiler Optimizations & Warnings
# ============================================================================

if(MSVC)
    # Visual Studio compiler
    add_compile_options(/W4 /permissive- /fp:fast)
    add_compile_options($<$<CONFIG:Release>:/O2 /Ot>)
    add_compile_options($<$<CONFIG:Debug>:/Od /Zi>)
else()
    # GCC/Clang compiler
    add_compile_options(-Wall -Wextra -Wpedantic)
    add_compile_options($<$<CONFIG:Release>:-O3>)
    add_compile_options($<$<CONFIG:Release>:-march=native>)
    add_compile_options($<$<CONFIG:Debug>:-g>)
    add_compile_options($<$<CONFIG:Debug>:-O0>)
endif()

# ============================================================================
# Dependencies
# ============================================================================

# Find required packages
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

# On Linux, we may need to link against libm (math library)
if(UNIX AND NOT APPLE)
    find_library(MATH_LIBRARY m)
endif()

# ============================================================================
# Source Files Organization
# ============================================================================

# Core source files
set(CORE_SOURCES
    src/main.cpp
    src/shaderClass.cpp
    src/texture.cpp
    src/camera.cpp
    src/shape.cpp
)

# Geometry/Shape implementations
set(SHAPE_SOURCES
    src/cube.cpp
    src/pyramid.cpp
    src/plane.cpp
    src/sphere.cpp
    src/cylinder.cpp
    src/TrapezoidPrism.cpp
)

# GPU buffer management
set(BUFFER_SOURCES
    src/VAO.cpp
    src/VBO.cpp
    src/EBO.cpp
)

# Third-party and utility sources
set(UTILITY_SOURCES
    src/glad.c
    src/stb.cpp
    src/include.cpp
)

# Combine all sources
set(PROJECT_SOURCES
    ${CORE_SOURCES}
    ${SHAPE_SOURCES}
    ${BUFFER_SOURCES}
    ${UTILITY_SOURCES}
)

# ============================================================================
# Header Directories
# ============================================================================

set(PROJECT_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# ============================================================================
# Main Executable
# ============================================================================

add_executable(ArtGallery ${PROJECT_SOURCES})

# Set target properties
set_target_properties(ArtGallery PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN ON
)

# Target include directories
target_include_directories(ArtGallery PRIVATE ${PROJECT_INCLUDE_DIRS})

# Link libraries - Fixed for Linux compatibility
target_link_libraries(ArtGallery PRIVATE
    glfw
    glm::glm
)

# Link OpenGL properly
if(APPLE)
    # macOS uses framework
    target_link_libraries(ArtGallery PRIVATE "-framework OpenGL")
elseif(WIN32)
    # Windows OpenGL
    target_link_libraries(ArtGallery PRIVATE OpenGL::GL)
else()
    # Linux - use direct GL library
    find_library(GL_LIBRARY GL)
    if(GL_LIBRARY)
        target_link_libraries(ArtGallery PRIVATE ${GL_LIBRARY})
    else()
        message(FATAL_ERROR "OpenGL library not found. Install: sudo apt-get install libgl1-mesa-dev")
    endif()
endif()

# Link math library on Linux
if(UNIX AND NOT APPLE AND MATH_LIBRARY)
    target_link_libraries(ArtGallery PRIVATE ${MATH_LIBRARY})
endif()

# ============================================================================
# Compiler-specific Optimizations
# ============================================================================

if(MSVC)
    # Additional MSVC optimizations
    target_compile_definitions(ArtGallery PRIVATE
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
else()
    # GCC/Clang specific
    if(CMAKE_BUILD_TYPE MATCHES Release)
        target_compile_options(ArtGallery PRIVATE
            -flto  # Link-time optimization
            -fvisibility=hidden
        )
        if(NOT APPLE)
            target_link_options(ArtGallery PRIVATE -flto)
        endif()
    endif()
endif()

# ============================================================================
# Assets Configuration
# ============================================================================

# Define asset directories for the running executable
set(ASSET_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
set(ASSET_DEST_DIR ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

# Copy assets to build directory after build
add_custom_command(TARGET ArtGallery POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${ASSET_SOURCE_DIR} ${ASSET_DEST_DIR}
    COMMENT "Copying assets to build directory..."
)

# ============================================================================
# Installation Configuration
# ============================================================================

# Install executable
install(TARGETS ArtGallery
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install assets
install(DIRECTORY assets/
    DESTINATION share/art-gallery/assets
    FILES_MATCHING PATTERN "*.png" PATTERN "*.jpg" PATTERN "*.vert" PATTERN "*.frag"
)

# ============================================================================
# Debugging & Development Options
# ============================================================================

option(ENABLE_SANITIZER "Enable AddressSanitizer for debugging" OFF)
if(ENABLE_SANITIZER AND NOT MSVC)
    target_compile_options(ArtGallery PRIVATE -fsanitize=address -fno-omit-frame-pointer)
    target_link_options(ArtGallery PRIVATE -fsanitize=address)
endif()

option(VERBOSE_LOGGING "Enable verbose logging in the application" OFF)
if(VERBOSE_LOGGING)
    target_compile_definitions(ArtGallery PRIVATE VERBOSE_LOGGING)
endif()

# ============================================================================
# Summary
# ============================================================================

message(STATUS "=== ArtGalleryRenderer Build Configuration ===")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Source directory: ${CMAKE_SOURCE_DIR}")
message(STATUS "Binary directory: ${CMAKE_BINARY_DIR}")
message(STATUS "Assets will be copied to: ${ASSET_DEST_DIR}")
message(STATUS "============================================")
