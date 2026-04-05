#pragma once

#include <string>

class Texture2D
{
public:
    unsigned int ID;
    int width, height;

    Texture2D(const std::string& path, unsigned int filter);
    ~Texture2D();

    void bind();
    void unbind();
};