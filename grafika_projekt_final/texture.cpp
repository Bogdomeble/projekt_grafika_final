#include "texture.h"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);
	GLenum local_format = format;

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);

    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);

    GLenum local_format_from_channels;
    if (numColCh == 1)
        local_format_from_channels = GL_RED;
    else if (numColCh == 3)
        local_format_from_channels = GL_RGB;  // <<< Jeśli numColCh to 3, to ustawi GL_RGB
    else if (numColCh == 4)
        local_format_from_channels = GL_RGBA;
    // ...

    GLenum internal_format_to_use = GL_RGBA8; // Domyślnie
    if (local_format_from_channels == GL_RGB) internal_format_to_use = GL_RGB8; // <<< Dla RGB8
    else if (local_format_from_channels == GL_RED) internal_format_to_use = GL_R8;

    glTexImage2D(type, 0, internal_format_to_use, widthImg, heightImg, 0, local_format_from_channels, GL_UNSIGNED_BYTE, bytes);
    

    // Mipmaps
    /*
    GL_NEAREST_MIPMAP_NEAREST: selects the nearest mipmap level matching the pixel size
    and uses nearest neighbor interpolation for texture sampling.
    GL_LINEAR_MIPMAP_NEAREST: selects the nearest mipmap level and samples that level
    using linear interpolation.
    GL_NEAREST_MIPMAP_LINEAR: linearly interpolates between the two mipmaps that most
    closely match the pixel size and samples the interpolated level using nearest neighbor.
    GL_LINEAR_MIPMAP_LINEAR: linearly interpolates between the two closest mipmaps
    and samples the interpolated level using linear interpolation.
    */

    glGenerateMipmap(texType);
    stbi_image_free(bytes);
    glBindTexture(texType, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.Activate();
    glUniform1i(texUni, unit);
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}