#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>
#include <SDL.h>
#include <iostream>
#include <stbi/stb_image.h>
#include <bitmap.hpp>
#include <GL/glew.h>

Bitmap::Bitmap(std::string filepath)
{
    SDL_RWops* file = SDL_RWFromFile(filepath.data(), "rb");

    if (file == nullptr)
    {
        throw std::runtime_error("Failed to open file: " + std::string{ filepath });
    }

    Sint64 length = SDL_RWsize(file);
    std::vector<unsigned char> data(length);

    SDL_RWread(file, data.data(), length, 1);
    SDL_RWclose(file);

    int width, height;
    stbi_set_flip_vertically_on_load(false);
    auto img = stbi_load_from_memory(data.data(), data.size(), &width, &height, &_colorChannels, 0);

    std::cout << width << " " << height << " " << _colorChannels << std::endl;

    if (img == nullptr)
    {
        throw std::runtime_error("Unsupported file format: " + std::string{ filepath });
    }

    //std::uint32_t id;
    //glGenTextures(1, &id);
    //glBindTexture(GL_TEXTURE_2D, id);
    
    /*auto internal_format = [&]()
    {
        switch (ch)
        {
        case 1: return GL_R8;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        }
    }();

    auto format = [&]()
    {
        switch (ch)
        {
        case 1: return GL_RED;
        case 2: return GL_RG;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        }
    }();

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, img);
    */
    auto imgSize = width * height * _colorChannels;

    _image = { img, img + imgSize };
    _size = glm::vec2(width, height);
    stbi_image_free(img);

    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, id);
}

const std::vector<unsigned char>& Bitmap::get_image() const
{
    return _image;
}

int Bitmap::get_color_channels() const
{
    return _colorChannels;
}

glm::vec2 Bitmap::get_size() const
{
    return _size;
}
