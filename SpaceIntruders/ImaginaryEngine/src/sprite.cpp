#include <sprite.hpp>
#include <engine.hpp>
#include <bitmap.hpp>
#include <window.hpp>
#include <shaderProgram.hpp>
#include <meshData.hpp>
#include <stdexcept>

Sprite::Sprite(const Engine& engine, std::string path, glm::vec2 repeat_amount)
    : Node(engine), _repeat_amount(repeat_amount)
{
    Bitmap bitmap(engine, path);

    _content_size = bitmap.get_size();

    MeshData data;

    std::vector<MeshData::Vertex> vertices;

    vertices.emplace_back();
    vertices.back().pos = { 0.0, 0.0 };
    vertices.back().texcoord = { 0.0, 0.0 };

    vertices.emplace_back();
    vertices.back().pos = { 0.0, bitmap.get_size().y };
    vertices.back().texcoord = { 0.0, repeat_amount.y };

    vertices.emplace_back();
    vertices.back().pos = { bitmap.get_size().x, bitmap.get_size().y };
    vertices.back().texcoord = { repeat_amount.x, repeat_amount.y };

    vertices.emplace_back();
    vertices.back().pos = { bitmap.get_size().x, 0.0 };
    vertices.back().texcoord = { repeat_amount.x, 0.0 };

    std::vector<std::uint32_t> indexes;

    indexes.emplace_back(0);
    indexes.emplace_back(2);
    indexes.emplace_back(3);

    indexes.emplace_back(0);
    indexes.emplace_back(1);
    indexes.emplace_back(2);

    data.vertices = std::move(vertices);
    data.indexes = std::move(indexes);

    _command.vertex_buffer = engine.get_render().create_vertex_buffer(std::move(data));
    _command.program = engine.get_render().create_program("draw");

    _texture_uniform = _command.program->create_texture_uniform("uTexture");
    _texture_uniform->texture = engine.get_render().create_texture(std::move(bitmap), _repeat_amount != glm::vec2(1.0f));

    _screen_size_uniform = _command.program->create_vec2_uniform("uScreenSize");
    _transform_uniform = _command.program->create_mat3_uniform("uTransform");

    _command.uniforms.push_back(_transform_uniform);
    _command.uniforms.push_back(_screen_size_uniform);
    _command.uniforms.push_back(_texture_uniform);
}

Sprite::Sprite(const Engine& engine, std::string path, float left, float right)
    : Node(engine)
{
    if (right - left == 0)
    {
        throw std::runtime_error("Height difference should not be equal to zero.");
    }

    Bitmap bitmap(engine, path);

    _content_size = bitmap.get_size();

    MeshData data;

    std::vector<MeshData::Vertex> vertices;


    if (left != 0)
    {
        vertices.emplace_back();
        vertices.back().pos = { 0.0, 1.0 - bitmap.get_size().y * left };
        vertices.back().texcoord = { 0.0, 1.0 - left };
    }

    vertices.emplace_back();
    vertices.back().pos = { 0.0, 0.0 };
    vertices.back().texcoord = { 0.0, 1.0 };

    vertices.emplace_back();
    vertices.back().pos = { bitmap.get_size().x, 0.0 };
    vertices.back().texcoord = { 1.0, 1.0 };

    if (right != 0)
    {
        vertices.emplace_back();
        vertices.back().pos = { bitmap.get_size().x, 1.0 - bitmap.get_size().y * right };
        vertices.back().texcoord = { 1.0, 1.0 - right };
    }

    std::vector<std::uint32_t> indexes;

    if (right != 0 && left != 0)
    {
        indexes.emplace_back(0);
        indexes.emplace_back(2);
        indexes.emplace_back(3);
    }

    indexes.emplace_back(0);
    indexes.emplace_back(1);
    indexes.emplace_back(2);

    data.vertices = std::move(vertices);
    data.indexes = std::move(indexes);

    _command.vertex_buffer = engine.get_render().create_vertex_buffer(std::move(data));
    _command.program = engine.get_render().create_program("draw");

    _texture_uniform = _command.program->create_texture_uniform("uTexture");
    _texture_uniform->texture = engine.get_render().create_texture(std::move(bitmap), true);

    _screen_size_uniform = _command.program->create_vec2_uniform("uScreenSize");
    _transform_uniform = _command.program->create_mat3_uniform("uTransform");

    _command.uniforms.push_back(_transform_uniform);
    _command.uniforms.push_back(_screen_size_uniform);
    _command.uniforms.push_back(_texture_uniform);
}

void Sprite::visitSelf()
{
    const auto& win = _engine.get_window();
    //_screen_size_uniform->value.x = (float)win.get_width();
    //_screen_size_uniform->value.y = (float)win.get_height();
    _screen_size_uniform->value = _engine.get_virtual_resolution();

    //_transform_uniform->value = this->get_transform();
    _transform_uniform->value = glm::inverse(_engine.get_camera()->get_transform()) * this->get_transform();
    _command.mask = _render_mask;

    _engine.get_render().add_command(_command);
}