#pragma once

#include <node.hpp>
#include <render.hpp>
#include <vector>
#include <string>

class Engine;
class VertexBuffer;
class ShaderProgram;
class TextureUniform;
class Mat3Uniform;
class Vec2Uniform;

class Sprite : public Node {
public:
	explicit Sprite(const Engine&, std::string);
//protected:
	void visitSelf() override;
private:
	//const Engine& _engine;
	Render::Command _command;

	std::shared_ptr<ShaderProgram> _program;
	std::shared_ptr<VertexBuffer> _vertex_buffer;
	std::shared_ptr<TextureUniform> _texture_uniform;
	std::shared_ptr<Mat3Uniform> _transform_uniform;
	std::shared_ptr<Vec2Uniform> _screen_size_uniform;
};