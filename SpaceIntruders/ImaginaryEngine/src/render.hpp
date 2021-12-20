#pragma once

#include <engine.hpp>
#include <meshData.hpp>
#include <bitmap.hpp>
#include <optional>
#include <glm/glm.hpp>

class VertexBuffer;
class ShaderProgram;
class Texture;

class Render
{
public:
    virtual ~Render() = default;

    struct Command
    {
        std::shared_ptr<VertexBuffer> vertex_buffer;
        std::shared_ptr<ShaderProgram> program;

        struct sub_t
        {
            size_t num;
            size_t offset;
        };

        std::optional<sub_t> sub;
        std::optional<glm::vec4> scissor;
    };

    void add_command(Command command) const
    {
        _commands.push_back(std::move(command));
    }

    virtual void draw() = 0;

    virtual std::shared_ptr<VertexBuffer> create_vertex_buffer(MeshData) const = 0;
    virtual std::shared_ptr<ShaderProgram> create_program(std::string) const = 0;
    virtual std::shared_ptr<Texture> create_texture(Bitmap) const = 0;
protected:
    mutable std::vector<Command> _commands;
};