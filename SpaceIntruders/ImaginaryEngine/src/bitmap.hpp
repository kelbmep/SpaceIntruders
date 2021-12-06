#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Bitmap
{
public:
    explicit Bitmap(std::string);

    const std::vector<unsigned char>& get_image() const;

    int get_color_channels() const;
    glm::vec2 get_size() const;

private:
    int _colorChannels;
    std::vector<unsigned char> _image;
    glm::vec2 _size;
};
