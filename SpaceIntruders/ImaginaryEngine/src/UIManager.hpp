#pragma once

#include <memory>
#include <render.hpp>
#include <node.hpp>

class Engine;
class EventManager;
class ShaderProgram;
class TextureUniform;
class Mat3Uniform;
class Vec2Uniform;

class UIManager : public EventManager::Delegate
{
public:
	explicit UIManager(const Engine&);
	void visit();
	void handle_event(EventManager::MouseEvent);
	void handle_event(EventManager::MouseMoveEvent);
	void handle_event(EventManager::TextInputEvent);
private:
	const Engine& _engine;
	Render::Command _command;
	std::shared_ptr<ShaderProgram> _program;

	bool _isLeft = false;
	bool _isRight = false;
	bool _isMiddle = false;

	glm::vec2 _mousePos;

	std::shared_ptr<TextureUniform> _texture_uniform;
	std::shared_ptr<Mat3Uniform> _transform_uniform;
	std::shared_ptr<Vec2Uniform> _screen_size_uniform;
};