#include <texture.hpp>

Texture::Texture(glm::vec2 size)
    : _size(size)
{

}

glm::vec2 Texture::get_size() const
{
    return _size;
}
