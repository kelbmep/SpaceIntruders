#pragma once 

//#include <cstddef>
#include <vector>
#include <glm/glm.hpp>

class MeshData
{
public:
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 texcoord;
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexes;
};