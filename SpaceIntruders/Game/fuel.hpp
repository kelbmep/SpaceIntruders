#pragma once

#include <node.hpp>
#include <box2d/box2d.h>

class Engine;
class UserData;

class Fuel : public Node
{
public:
	Fuel(const Engine&, UserData&, float, float);

	~Fuel() = default;
private:
	UserData& _userdata;
	b2Body* _body;
protected:
	void visitSelf() override;
};