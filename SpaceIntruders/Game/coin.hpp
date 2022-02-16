#pragma once

#include <node.hpp>
#include <box2d/box2d.h>

class Engine;
class UserData;

class Coin : public Node
{
public:
	Coin(const Engine&, UserData&, float, float);
	
	~Coin() = default;
private:
	UserData& _userdata;

	float _radius = 1.0f;
	int _money = 0;

	b2Body* _body;
protected:
	void visitSelf() override;
};