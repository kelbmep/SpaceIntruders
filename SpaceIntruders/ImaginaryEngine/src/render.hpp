#pragma once

#include <engine.hpp>

class Render
{
public:
    virtual ~Render() = default;
    virtual void draw() = 0;
    void load_picture(std::vector<Engine::VertexTriangle>);
protected:
    std::vector<Engine::VertexTriangle> _picture;
};