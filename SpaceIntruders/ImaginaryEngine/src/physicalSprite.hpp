#pragma once

#include <box2d/box2d.h>
#include <sprite.hpp>
#include <memory>

class Engine;

class PhysicalSprite : public Sprite
{
public:
	PhysicalSprite(const Engine&, b2Body*, std::string);
	b2Body* get_body() const;
protected:
	void visitSelf() override;
private:
	b2Body* _body;
	float _angle = 0.0f;
};