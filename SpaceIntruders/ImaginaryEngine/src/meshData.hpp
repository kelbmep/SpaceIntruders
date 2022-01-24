#pragma once 

#include <vector>
#include <utilities/general_functions.hpp>

class MeshData
{
public:
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 texcoord;
        utils::color color{ glm::vec4(1.0f) };
    };

    std::vector<Vertex> vertices;
    std::vector<uint32_t> indexes;
};