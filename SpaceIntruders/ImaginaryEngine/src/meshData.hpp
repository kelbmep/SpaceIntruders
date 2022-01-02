#pragma once 

#include <vector>
#include <glm/glm.hpp>
#include <vertexBuffer.hpp>

class MeshData
{
public:
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 texcoord;
        VertexBuffer::color color{ glm::vec4(1.0f, 1.0f, 1.0f, 1.0f) };
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexes;
};