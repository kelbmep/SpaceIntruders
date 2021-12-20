#pragma once

#include <../src/eventManager.hpp>
#include <../src/node.hpp>
#include <chrono>
#include <memory>

class Engine;
class Sound;
class Sprite;

class Tank : public EventManager::Delegate, public Node
{
public:
	explicit Tank(const Engine&);
	
	~Tank();
	
	void handle_event(EventManager::QuitEvent);
	void handle_event(EventManager::KeyEvent);
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
	float _turn_speed = 0;
	
	std::shared_ptr<Sound> _shot;
	std::shared_ptr<Sound> _movement;
protected:
	void visitSelf() override;
};
