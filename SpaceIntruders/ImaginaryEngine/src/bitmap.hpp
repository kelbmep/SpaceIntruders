#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>

class Engine;

class Bitmap
{
public:
    Bitmap(const Engine&, std::string);
    explicit Bitmap(int, std::vector<unsigned char>, glm::vec2);

    const std::vector<unsigned char>& get_image() const;

    int get_color_channels() const;
    glm::vec2 get_size() const;

private:
    int _color_channels;
    std::vector<unsigned char> _image;
    glm::vec2 _size;
};
