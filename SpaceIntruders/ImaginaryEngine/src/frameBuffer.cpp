#include <frameBuffer.hpp>

std::shared_ptr<Texture> FrameBuffer::get_texture() const
{
    return _texture;
}