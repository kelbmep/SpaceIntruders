#pragma once

#include <node.hpp>
#include <engine.hpp>
#include <vector>

class Engine;
class Texture;
class VertexBuffer;
class ShaderProgram;
class TextureUniform;
class Mat3Uniform;
class Vec2Uniform;

class Sprite : public Node {
public:
	explicit Sprite(const Engine&, std::string);
protected:
	void visitSelf() override;
private:
	const Engine& _engine;

	std::shared_ptr<ShaderProgram> _program;
	std::shared_ptr<VertexBuffer> _vertexBuffer;
	std::shared_ptr<TextureUniform> _textureUniform;
	std::shared_ptr<Mat3Uniform> _transformUniform;
	std::shared_ptr<Vec2Uniform> _screenSizeUniform;
};