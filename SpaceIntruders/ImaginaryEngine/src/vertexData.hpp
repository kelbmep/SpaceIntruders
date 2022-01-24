#include <array>
#include <vector>
#include <glm/glm.hpp>

struct VertexLayout
{
    enum Type
    {
        Float, Vec2, Vec3, Vec4, Color
    } type{};
    bool normalized{};
};

struct VertexData
{
    virtual const void* data() const = 0;
    virtual size_t dataSize() const = 0;
    virtual size_t vertexSize() const = 0;

    virtual VertexLayout layoutFor(size_t) const = 0;
    virtual size_t offsetFor(size_t) const = 0;
    virtual size_t componentsNum() const = 0;

    std::array<uint32_t, 32> indexes;
};
