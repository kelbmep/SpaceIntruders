#define STB_IMAGE_IMPLEMENTATION
#include <stdexcept>
#include <SDL.h>
#include <stbi/stb_image.h>
#include <bitmap.hpp>

Bitmap::Bitmap(std::string filepath)
{
    SDL_RWops* file = SDL_RWFromFile(filepath.data(), "rb");

    if (file == nullptr)
    {
        throw std::runtime_error("Failed to open file: " + std::string{ filepath });
    }

    auto length = SDL_RWsize(file);
    std::vector<unsigned char> data(length);

    SDL_RWread(file, data.data(), length, 1);
    SDL_RWclose(file);

    int width, height;
    stbi_set_flip_vertically_on_load(false);
    auto img = stbi_load_from_memory(data.data(), (int)data.size(), &width, &height, &_color_channels, 0);

    if (img == nullptr)
    {
        throw std::runtime_error("Unsupported file format: " + std::string{ filepath });
    }

    auto imgSize = width * height * _color_channels;

    _image = { img, img + imgSize };
    _size = glm::vec2(width, height);
    stbi_image_free(img);
}

Bitmap::Bitmap(int color_channels, std::vector<unsigned char> data, glm::vec2 size)
    :_color_channels(color_channels), _image(std::move(data)), _size(size)
{
}

const std::vector<unsigned char>& Bitmap::get_image() const
{
    return _image;
}

int Bitmap::get_color_channels() const
{
    return _color_channels;
}

glm::vec2 Bitmap::get_size() const
{
    return _size;
}
