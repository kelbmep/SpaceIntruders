#pragma once

#include <frameBuffer.hpp>
#include <glm/glm.hpp>

class Engine;
class GLFrameBuffer final : public FrameBuffer
{
public:
    explicit GLFrameBuffer(const Engine&, glm::vec2);
    void draw_to_frame_buffer(std::function<void(void)>) override;

    virtual ~GLFrameBuffer();
private:
    uint32_t _fbo;
};
