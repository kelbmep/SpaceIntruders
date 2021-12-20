#include <sprite.hpp>
#include <engine.hpp>
#include <bitmap.hpp>
#include <render.hpp>
#include <window.hpp>
#include <shaderProgram.hpp>
#include <meshData.hpp>

Sprite::Sprite(const Engine& engine, std::string path)
    : _engine(engine)
{
    Bitmap bitmap(path);

    _content_size = bitmap.get_size();

    MeshData data;

    data.vertices.emplace_back();
    data.vertices.back().pos = { 0.0, 0.0 };
    data.vertices.back().texcoord = { 0.0, 0.0 };

    data.vertices.emplace_back();
    data.vertices.back().pos = { 0.0, bitmap.get_size().y };
    data.vertices.back().texcoord = { 0.0, 1.0 };

    data.vertices.emplace_back();
    data.vertices.back().pos = { bitmap.get_size().x, bitmap.get_size().y };
    data.vertices.back().texcoord = { 1.0, 1.0 };

    data.vertices.emplace_back();
    data.vertices.back().pos = { bitmap.get_size().x, 0.0 };
    data.vertices.back().texcoord = { 1.0, 0.0 };

    data.indexes.emplace_back(0);
    data.indexes.emplace_back(2);
    data.indexes.emplace_back(3);

    data.indexes.emplace_back(0);
    data.indexes.emplace_back(1);
    data.indexes.emplace_back(2);

    _vertex_buffer = engine.get_render().create_vertex_buffer(std::move(data));
    _program = engine.get_render().create_program("draw");

    _texture_uniform = _program->create_texture_uniform("uTexture");
    _texture_uniform->texture = engine.get_render().create_texture(std::move(bitmap));

    _screen_size_uniform = _program->create_vec2_uniform("uScreenSize");
    _transform_uniform = _program->create_mat3_uniform("uTransform");
}

void Sprite::visitSelf()
{
    const auto& win = _engine.get_window();
    _screen_size_uniform->value.x = (float)win.get_width();
    _screen_size_uniform->value.y = (float)win.get_height();

    _transform_uniform->value = this->get_transform();

    Render::Command command;
    command.program = _program;
    command.vertex_buffer = _vertex_buffer;
    _engine.get_render().add_command(std::move(command));
}