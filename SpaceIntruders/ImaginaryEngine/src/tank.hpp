#pragma once

#include <eventManager.hpp>
#include <node.hpp>
#include <engine.hpp>
#include <sprite.hpp>
#include <chrono>
#include <glm/gtx/rotate_vector.hpp>

class Tank : public EventManager::Delegate, public Node
{
public:
	explicit Tank(const Engine&);
	
	void handle_event(EventManager::QuitEvent);
	void handle_event(EventManager::KeyEvent);
	/*void handle_event(EventManager::KeyUpEvent);
	void handle_event(EventManager::KeyDownEvent);
	void handle_event(EventManager::KeyLeftEvent);
	void handle_event(EventManager::KeyRightEvent);
	void handle_event(EventManager::KeyAEvent);
	void handle_event(EventManager::KeyDEvent);*/
	void visitSelf() override;
private:
	//std::shared_ptr<EventManager> _eventManager;
	//const Engine& _engine;

	bool _isUp = false;
	bool _isDown = false;
	bool _isLeft = false;
	bool _isRight = false;
	bool _isA = false;
	bool _isD = false;
	
	std::shared_ptr<Sprite> _body;
	std::shared_ptr<Sprite> _tower;
	std::chrono::high_resolution_clock::time_point _prev_time;// = std::chrono::steady_clock();
	float _speed = 0;
	float _rotation = 0;
	float _turnSpeed = 0;
};
