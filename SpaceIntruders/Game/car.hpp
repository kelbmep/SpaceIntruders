#pragma once

#include <../src/eventManager.hpp>
#include <../src/node.hpp>
#include <box2d/box2d.h>
#include <chrono>
#include <memory>

class Engine;
class UserData;
class Sound;
class PhysicalSprite;

class Car : public EventManager::Delegate, public Node
{
public:
	explicit Car(const Engine&, UserData&, float, float);
	
	~Car() = default;

	std::shared_ptr<PhysicalSprite> get_car_body() const;
	void handle_event(KeyEvent) override;
private:
	UserData& _userdata;

	bool _isLeft = false;
	bool _isRight = false;

	std::shared_ptr<PhysicalSprite> _car;
	std::shared_ptr<PhysicalSprite> _back_wheel;
	std::shared_ptr<PhysicalSprite> _front_wheel;

	b2WheelJoint* _m_spring1;
	b2WheelJoint* _m_spring2;

	std::chrono::high_resolution_clock::time_point _prev_time;

	float _speed = 0;
	float _scale = 150.0f;

	std::shared_ptr<Sound> _movement;
protected:
	void visitSelf() override;
};