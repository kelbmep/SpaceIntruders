#pragma once

#include <../src/eventManager.hpp>
#include <../src/node.hpp>
#include <../src/engine.hpp>
#include <../src/sprite.hpp>
#include <chrono>

class Bullet : public Node
{
public:
	//explicit Bullet();
	explicit Bullet(const Engine&, glm::vec2);
	//void handle_event(EventManager::QuitEvent);
	//void handle_event(EventManager::KeyEvent);
	//void visitSelf() override;
private:
	//std::shared_ptr<EventManager> _eventManager;
	//const Engine& _engine;
	std::shared_ptr<Sprite> _bullet;

	//std::chrono::high_resolution_clock::time_point _prev_time;// = std::chrono::steady_clock();
	//float _speed = 0;
	//float _rotation = 0;
	//float _turnSpeed = 0;
};
