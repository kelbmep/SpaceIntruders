#pragma once

#include <GL/GLRender.hpp>
#include <engine.hpp>
#include <../libs/glm/gtx/matrix_transform_2d.hpp>

class Sprite {
public:
	explicit Sprite() {};
	Sprite(std::vector<Engine::VertexTriangle> tr) 
	{
		_triangles = tr;
	}
	void Transform(float scale = 1.0, int rotation = 0, glm::vec2 pos = { 0, 0 });//pos_x = 0, int pos_y = 0);
	//glm::mat3 getTransform();
	void push(Engine::VertexTriangle);
	const Engine::VertexTriangle* get_data() const;
	size_t get_size() const;
	float get_scale()
	{
		return _scale;
	}
	int get_rotation()
	{
		return _rotation;
	}
	glm::vec2 get_position()
	{
		return _position;
	}
private:
	std::vector<Engine::VertexTriangle> _triangles;
	//glm::vec2 _scale = { 1.0, 1.0 };
	float _scale = 1.0;
	int _rotation = 0;
	glm::vec2 _position = { 0.0, 0.0 };
};