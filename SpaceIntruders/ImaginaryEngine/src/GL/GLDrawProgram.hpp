#pragma once

#include <GL/GlProgram.hpp>
#include <GL/GlTexture.hpp>
#include <eventManager.hpp>
#include <cstdint>

class Engine;

class GLDrawProgram final : public GLProgram
{
public:
    explicit GLDrawProgram();
};