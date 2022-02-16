#pragma once

#include <../src/sprite.hpp>
#include <chrono>

class Bullet : public Node
{
public:
	explicit Bullet(const Engine&, const glm::vec2&);
private:
	std::shared_ptr<Sprite> _bullet;
		
	std::chrono::high_resolution_clock::time_point _prev_time;

	float _total_time = 0;
	
	glm::vec2 _speed = glm::vec2(0.0f);
protected:
	void visitSelf() override;
};
