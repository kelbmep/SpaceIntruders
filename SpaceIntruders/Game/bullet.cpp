#include "bullet.hpp"
#include <glm/gtx/rotate_vector.hpp>

Bullet::Bullet(const Engine& engine, glm::vec2 pos) 
{
    _bullet = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_bullet.png");
    
    this->set_position(pos);

    this->addNode(_bullet);
    //_body->addNode(_tower);
}
