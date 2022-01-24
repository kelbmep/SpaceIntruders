#pragma once

#include <vertexBuffer.hpp>
#include <meshData.hpp>
#include <render.hpp>
//#include <vertexData.hpp>

class Engine;

class GLVertexBuffer final : public VertexBuffer
{
public:
    explicit GLVertexBuffer(const Engine&, MeshData);
    explicit GLVertexBuffer(const Engine&, const VertexData&);
    virtual ~GLVertexBuffer();
    void draw();
    void draw(size_t, size_t);
private:
    uint32_t _VAO = -1;
    uint32_t _VBO = -1;
    uint32_t _IBO = -1;

    uint32_t _count;
    const Engine& _engine;
};
