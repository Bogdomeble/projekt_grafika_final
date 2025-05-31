#include "texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;
    int widthImg, heightImg, numColCh;

    // Flip image vertically on load to match OpenGL's coordinate system
    stbi_set_flip_vertically_on_load(true);

    // Load image data
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
    GLenum local_format = format;

    // Generate and bind texture
    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    // Set texture filtering parameters
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Set texture wrapping parameters
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Determine format based on number of color channels
    GLenum local_format_from_channels = GL_RGB;
    if (numColCh == 1)
        local_format_from_channels = GL_RED;
    else if (numColCh == 3)
        local_format_from_channels = GL_RGB;
    else if (numColCh == 4)
        local_format_from_channels = GL_RGBA;

    // Choose internal format for OpenGL
    GLenum internal_format_to_use = GL_RGBA8; // Default
    if (local_format_from_channels == GL_RGB)
        internal_format_to_use = GL_RGB8;
    else if (local_format_from_channels == GL_RED)
        internal_format_to_use = GL_R8;

    // Upload texture data to GPU
    glTexImage2D(
        type,
        0,
        internal_format_to_use,
        widthImg,
        heightImg,
        0,
        local_format_from_channels,
        GL_UNSIGNED_BYTE,
        bytes
    );

    // Generate mipmaps for the texture
    /*
        GL_NEAREST_MIPMAP_NEAREST: selects the nearest mipmap level matching the pixel size and uses nearest neighbor interpolation for texture sampling.
        GL_LINEAR_MIPMAP_NEAREST: selects the nearest mipmap level and samples that level using linear interpolation.
        GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most closely match the pixel size and samples the interpolated level using nearest neighbor.
        GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps and samples the interpolated level using linear interpolation.
    */
    glGenerateMipmap(texType);

    // Free image memory and unbind texture
    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    // Set the texture unit for the shader uniform
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    // Bind the texture
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    // Unbind the texture
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    // Delete the texture from GPU memory
    glDeleteTextures(1, &ID);
}