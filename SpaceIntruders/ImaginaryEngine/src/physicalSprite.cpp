#include <physicalSprite.hpp>

PhysicalSprite::PhysicalSprite(Engine& engine, b2Body* body, std::shared_ptr<Sprite> sprite)
	: Node(engine), 
	  _body(body), 
	  _sprite(sprite)
{
}

std::shared_ptr<Sprite> PhysicalSprite::get_sprite() const
{
	return _sprite;
}

void PhysicalSprite::visitSelf()
{
	_sprite->visitSelf();

	float x = _body->GetPosition().x;
	float y = _body->GetPosition().y;

	_sprite->set_position(glm::vec2(x, y));
	_sprite	->set_rotation(_body->GetAngle());
}