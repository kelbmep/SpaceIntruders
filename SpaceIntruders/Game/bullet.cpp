#include "bullet.hpp"
#include <../src/engine.hpp>
#include <glm/gtx/vector_angle.hpp>

Bullet::Bullet(const Engine& engine, const glm::vec2 &speed) : _speed(speed)
{
    _bullet = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_bullet.png");
    _bullet->set_scale(glm::vec2(1.5f));
    this->add_node(_bullet);
    _prev_time = std::chrono::high_resolution_clock::now();
}

void Bullet::visitSelf()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - _prev_time;
    _prev_time = now;

    _total_time += delta.count();
    
    if (_total_time >= 1.5f)
    {
        //this->remove_from_parent();
        return;
    }
    
    set_position(get_position() + delta.count() + 3.0f * _speed);
    set_rotation(glm::degrees(glm::orientedAngle(glm::vec2(0.0f, -1.0f), glm::normalize(_speed))));
}