#include <sprite.hpp>
#include <bitmap.hpp>
#include <render.hpp>
#include <window.hpp>
#include <shaderProgram.hpp>
#include <meshData.hpp>

Sprite::Sprite(const Engine& engine, std::string path)
    : _engine(engine)
{
    Bitmap bitmap(path);

    _contentSize = bitmap.get_size();

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

    _vertexBuffer = engine.get_render().create_vertex_buffer(std::move(data));
    _program = engine.get_render().create_program("draw");

    _textureUniform = _program->createTextureUniform("uTexture");
    _textureUniform->texture = engine.get_render().create_texture(std::move(bitmap));

    _screenSizeUniform = _program->createVec2Uniform("uScreenSize");
    _transformUniform = _program->createMat3Uniform("uTransform");
}

void Sprite::visitSelf()
{
    const auto& win = _engine.get_window();
    _screenSizeUniform->value.x = win.get_width();
    _screenSizeUniform->value.y = win.get_height();

    _transformUniform->value = this->get_transform();

    Render::Command command;
    command.program = _program;
    command.vertexBuffer = _vertexBuffer;
    _engine.get_render().add_command(std::move(command));
}
