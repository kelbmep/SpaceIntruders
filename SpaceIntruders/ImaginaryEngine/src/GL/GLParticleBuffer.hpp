#pragma once

#include <particleBuffer.hpp>
#include <vector>

class GLParticleBuffer : public ParticleBuffer
{
public:
    explicit GLParticleBuffer(std::vector<ParticleData>);

    explicit GLParticleBuffer(size_t);
    void add_data(ParticleData) override;

    virtual ~GLParticleBuffer();

    void draw();
private:
    uint32_t _VAO = 0;
    uint32_t _VBO = 0;

    uint32_t _count;

    bool _is_dynamic = false;
    
    uint32_t _capacity = 0;
    uint32_t _cur_pos = 0;

    void create_buffers();
    void create_layouts();
};