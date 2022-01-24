#pragma once

#include <vertexBuffer.hpp>

namespace detail
{
    template<size_t Index, typename ArrayType, typename T, typename... Types>
    constexpr auto genLayoutImpl(ArrayType& layout)
    {
        auto& cur = layout[Index];
        if constexpr (std::is_same_v<T, float>)
        {
            cur.type = VertexLayout::Type::Float;
            cur.normalized = false;
        }
        else if constexpr (std::is_same_v<T, glm::vec2>)
        {
            cur.type = VertexLayout::Type::Vec2;
            cur.normalized = false;
        }
        else if constexpr (std::is_same_v<T, glm::vec3>)
        {
            cur.type = VertexLayout::Type::Vec3;
            cur.normalized = false;
        }
        else if constexpr (std::is_same_v<T, glm::vec4>)
        {
            cur.type = VertexLayout::Type::Vec4;
            cur.normalized = false;
        }
        else if constexpr (std::is_same_v<T, color>)
        {
            cur.type = VertexLayout::Type::Color;
            cur.normalized = true;
        }

        if constexpr ((Index + 1) < std::tuple_size<ArrayType>::value)
        {
            genLayoutImpl<Index + 1, ArrayType, Types...>(layout);
        }

    }

    template<typename... Types>
    constexpr auto genLayout()
    {
        std::array<VertexLayout, sizeof...(Types)> layout;
        genLayoutImpl<0, std::array<VertexLayout, sizeof...(Types)>, Types...>(layout);
        return layout;
    }
}

template<typename... Types>
struct LayoutGen
{
    static constexpr std::array<VertexLayout, sizeof...(Types)> layout = detail::genLayout<Types...>();
};

template<size_t... Offsets>
struct OffsetsGen
{
    static constexpr std::array<size_t, sizeof...(Offsets)> offsets = { Offsets... };
};

template<typename T, typename Layout, typename Offsets>
struct VertexDataCreator final : VertexData
{
    static_assert(std::tuple_size<decltype(Layout::layout)>::value == std::tuple_size<decltype(Offsets::offsets)>::value);

    const void* data() const override
    {
        return vertices.get_data();
    }

    size_t dataSize() const override
    {
        return vertices.get_size() * sizeof(T);
    }

    size_t vertexSize() const override
    {
        return sizeof(T);
    }

    size_t componentsNum() const override
    {
        return std::tuple_size<decltype(Layout::layout)>::value;
    }

    VertexLayout layoutFor(size_t index) const override
    {
        return Layout::layout.at(index);
    }

    size_t offsetFor(size_t index) const override
    {
        return Offsets::offsets.at(index);
    }

    std::array<T, sizeof(T)> vertices;
};