#pragma once

#include <box2d/box2d.h>

class Engine;
class Sprite;

class Ground
{
public:
	Ground(const Engine&, float);
	float calculate_height(float) const;
private:
	b2Body* _ground = NULL;
	float _ground_level = 720.0f;
};