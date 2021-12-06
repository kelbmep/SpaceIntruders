#pragma once

#include <vertexBuffer.hpp>
#include <meshData.hpp>

class Engine;
class GLVertexBuffer final : public VertexBuffer
{
public:
    explicit GLVertexBuffer(const Engine&, MeshData);
    virtual ~GLVertexBuffer();
    void draw();
private:
    uint32_t _VAO = 0;
    uint32_t _VBO = 0;
    uint32_t _IBO = 0;

    uint32_t _count;
    const Engine& _engine;
};
