#pragma once

#include <../src/eventManager.hpp>
#include <../src/node.hpp>
#include <../src/engine.hpp>
#include <../src/sprite.hpp>
#include <chrono>
#include <../src/audioManager.hpp>
#include <vector>

class Tank : public EventManager::Delegate, public Node
{
public:
	explicit Tank(const Engine&);
	~Tank();
	void handle_event(EventManager::QuitEvent);
	void handle_event(EventManager::KeyEvent);
	void visitSelf() override;
private:
	const Engine& _engine;
	
	bool _isUp = false;
	bool _isDown = false;
	bool _isLeft = false;
	bool _isRight = false;
	bool _isA = false;
	bool _isD = false;
	
	std::shared_ptr<Sprite> _body;
	std::shared_ptr<Sprite> _tower;
	std::chrono::high_resolution_clock::time_point _prev_time;
	float _speed = 0;
	float _rotation = 0;
	float _turnSpeed = 0;
	
	std::shared_ptr<Sound> _shot;
	std::shared_ptr<Sound> _movement;
};
