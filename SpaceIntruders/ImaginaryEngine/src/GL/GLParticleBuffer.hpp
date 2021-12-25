#pragma once

#include <particleBuffer.hpp>
#include <vector>

class GLParticleBuffer : public ParticleBuffer
{
public:
	explicit GLParticleBuffer(std::vector<ParticleData>);
    void draw();
private:
    uint32_t _VAO = 0;
    uint32_t _VBO = 0;
    
    uint32_t _count;
    //const Engine& _engine;
};