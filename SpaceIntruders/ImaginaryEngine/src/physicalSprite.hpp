#pragma once

#include <box2d/box2d.h>
#include <sprite.hpp>
#include <memory>

class Engine;

class PhysicalSprite : public Node
{
public:
	PhysicalSprite(Engine&, b2Body*, std::shared_ptr<Sprite>);
	std::shared_ptr<Sprite> get_sprite() const;
protected:
	void visitSelf() override;
private:
	std::shared_ptr<Sprite> _sprite;
	b2Body* _body;
};