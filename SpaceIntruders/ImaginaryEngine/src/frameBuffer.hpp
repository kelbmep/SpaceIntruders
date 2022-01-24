#pragma once

#include <memory>
#include <functional>

class Texture;
class FrameBuffer
{
public:
    virtual ~FrameBuffer() = default;
    std::shared_ptr<Texture> get_texture() const;

    virtual void draw_to_frame_buffer(std::function<void(void)>) = 0;
protected:
    std::shared_ptr<Texture> _texture;
};