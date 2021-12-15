#pragma once

/*#include <engine.hpp>

class Render
{
public:
    virtual ~Render() = default;
    virtual void draw() = 0;
    
protected:
    std::vector<Engine::VertexTriangle> _picture;
};*/

//#include <vector>
//#include <memory>
//#include <string>
#include <engine.hpp>
#include <meshData.hpp>
#include <bitmap.hpp>

class VertexBuffer;
class ShaderProgram;
class Texture;

class Render
{
public:
    virtual ~Render() = default;

    struct Command
    {
        std::shared_ptr<VertexBuffer> vertexBuffer;
        std::shared_ptr<ShaderProgram> program;
    };

    void add_command(Command command) const
    {
        _commands.push_back(std::move(command));
    }

    virtual void draw() = 0;
    virtual void draw(size_t, size_t) = 0;

    virtual std::shared_ptr<VertexBuffer> create_vertex_buffer(MeshData) const = 0;
    virtual std::shared_ptr<ShaderProgram> create_program(std::string) const = 0;
    virtual std::shared_ptr<Texture> create_texture(Bitmap) const = 0;
protected:
    mutable std::vector<Command> _commands;
};