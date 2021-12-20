#pragma once

#include <memory>
#include <render.hpp>

class Engine;
class EventManager;
class ShaderProgram;
class TextureUniform;
class Mat3Uniform;
class Vec2Uniform;

class UIManager final 
{
public:
	explicit UIManager(const Engine&);
	void visit();
private:
	const Engine& _engine;
	Render::Command _command;
	std::shared_ptr<ShaderProgram> _program;

	std::shared_ptr<TextureUniform> _texture_uniform;
	std::shared_ptr<Mat3Uniform> _transform_uniform;
	std::shared_ptr<Vec2Uniform> _screen_size_uniform;
};