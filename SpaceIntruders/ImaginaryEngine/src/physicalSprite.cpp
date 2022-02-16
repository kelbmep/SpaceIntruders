#include <physicalSprite.hpp>

PhysicalSprite::PhysicalSprite(const Engine& engine, b2Body* body, std::string filepath)
	: Sprite(engine, filepath), 
	  _body(body)
{
}

b2Body* PhysicalSprite::get_body() const
{
	return _body;
}

void PhysicalSprite::visitSelf()
{
	Sprite::visitSelf();

	this->set_position(glm::vec2(_body->GetPosition().x, _body->GetPosition().y));
	this->set_rotation(glm::degrees(_body->GetAngle()));
}