#include <core/textrure2D.hpp>
#include <lib/stb_image.h>
#include <glad/glad.h>
#include <iostream>

Texture2D::Texture2D(const std::string& path, unsigned int filter)
{
    int channels;
    unsigned char* data = stbi_load(path.c_str(), &this->width, &this->height, &channels, 4);

    if (!data)
    {
        std::cerr << "Cannot load texture: " << path << std::endl;
        return;
    }

    stbi_set_flip_vertically_on_load(true);

    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    stbi_image_free(data);
}

Texture2D::~Texture2D()
{
    glDeleteTextures(1, &ID);
}

void Texture2D::bind()
{
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture2D::unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}