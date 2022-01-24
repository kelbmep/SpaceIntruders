#pragma once

#include <engine.hpp>
#include <bitmap.hpp>
#include <optional>
#include <glm/glm.hpp>
#include <vertexData.hpp>
#include <particleBuffer.hpp>

#include <meshData.hpp>

class VertexBuffer;
class ShaderProgram;
class Texture;
class Uniform;

class Render
{
public:
    virtual ~Render() = default;
    virtual void init() = 0;

    struct Command
    {
        uint32_t mask;
        std::shared_ptr<VertexBuffer> vertex_buffer;
        std::shared_ptr<ShaderProgram> program;
        std::vector<std::shared_ptr<Uniform>> uniforms;

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
    virtual std::shared_ptr<VertexBuffer> create_vertex_buffer(const VertexData&) const = 0;
    virtual std::shared_ptr<ParticleBuffer> create_particle_buffer(std::vector<ParticleBuffer::ParticleData>) const = 0;
    virtual std::shared_ptr<ParticleBuffer> create_particle_buffer(size_t) const = 0;
    
    virtual std::shared_ptr<ShaderProgram> create_program(std::string) const = 0;
    virtual std::shared_ptr<Texture> create_texture(Bitmap) const = 0;
    
    mutable glm::vec3 distort_coord;
    
    virtual glm::vec2 get_render_resolution() const = 0;
protected:
    mutable std::vector<Command> _commands;
};