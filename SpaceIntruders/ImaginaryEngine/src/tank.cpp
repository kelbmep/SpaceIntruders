#include <tank.hpp>
#include <iostream>

Tank::Tank(const Engine& engine)
{
    _body = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_body.png");
    _tower = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_tower.png");
    _body->set_position(glm::vec2(engine.get_window_width() * 0.5f,
                                  engine.get_window_height() * 0.5f));

    _body->set_scale(glm::vec2(0.4f, 0.4f));

    glm::vec2 tank_body_size = _body->get_size();
    glm::vec2 tank_tower_size = _tower->get_size();
    
    _tower->set_position(glm::vec2(tank_body_size.x * 0.5f, tank_body_size.y * 0.539f));
    _tower->set_anchor(glm::vec2(0.5f, 0.694f));

    this->addNode(_body);
    _body->addNode(_tower);
}
/*
void Tank::handle_event(EventManager::KeyUpEvent ev)
{
    _isUp = true;
    std::cout << "Up";
}

void Tank::handle_event(EventManager::KeyDownEvent ev)
{
    _isDown = true;
    std::cout << "Down";
}

void Tank::handle_event(EventManager::KeyLeftEvent ev)
{
    _isLeft = true;
    std::cout << "Left";
}

void Tank::handle_event(EventManager::KeyRightEvent ev)
{
    _isRight = true;
    std::cout << "Right";
}

void Tank::handle_event(EventManager::KeyAEvent ev)
{
    _isA = true;
    std::cout << "AAA";
}

void Tank::handle_event(EventManager::KeyDEvent ev)
{
    _isD = true;
    std::cout << "DDD";
}*/

void Tank::handle_event(EventManager::QuitEvent ev)
{
    std::cout << "Iquit";
}

void Tank::handle_event(EventManager::KeyEvent ev)
{
    if (ev.key == EventManager::KeyCode::Up)
    {
        _isUp = (ev.type == EventManager::KeyType::KeyDown);
    }

    if (ev.key == EventManager::KeyCode::Down)
    {
        _isDown = (ev.type == EventManager::KeyType::KeyDown);
    }

    if (ev.key == EventManager::KeyCode::Left)
    {
        _isLeft = (ev.type == EventManager::KeyType::KeyDown);
    }

    if (ev.key == EventManager::KeyCode::Right)
    {
        _isRight = (ev.type == EventManager::KeyType::KeyDown);
    }

    if (ev.key == EventManager::KeyCode::A)
    {
        _isA = (ev.type == EventManager::KeyType::KeyDown);
    }

    if (ev.key == EventManager::KeyCode::D)
    {
        _isD = (ev.type == EventManager::KeyType::KeyDown);
    }
}

void Tank::visitSelf()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - _prev_time;
    _prev_time = now;

    //constexpr auto barrelSpeed = 70.0f;
    
    if (_isA) 
    {
        _tower->set_rotation(_tower->get_rotation() - 0.3);
    }
    if (_isD) 
    {
        _tower->set_rotation(_tower->get_rotation() + 0.3);
    }

    if (_isLeft)
    {
        if (std::abs(_turnSpeed) < 70.0f)
        {
            _turnSpeed += delta.count() * 120.0f + _speed * 0.002f;
        }

        _transform = std::nullopt;
    }
    if (_isRight)
    {
        if (std::abs(_turnSpeed) < 70.0f)
        {
            _turnSpeed -= delta.count() * 120.0f + _speed * 0.002f;
        }

        _transform = std::nullopt;
    }

    if (_turnSpeed > 0.1f)
    {
        _rotation -= delta.count() * _turnSpeed;
        _turnSpeed -= delta.count() * 115.0f;
        _transform = std::nullopt;
        if (_turnSpeed <= -0.1f)
        {
            _turnSpeed = 0.0f;
        }
    }

    if (_turnSpeed < -0.1f)
    {
        _rotation -= delta.count() * _turnSpeed;
        _turnSpeed += delta.count() * 115.0f;
        _transform = std::nullopt;
        if (_turnSpeed >= 0.1f)
        {
            _turnSpeed = 0.0f;
        }
    }

    if (_isUp)
    {
        if (_speed < 500.0f)
        {
            _speed += delta.count() * 460.0f;
        }
    }

    if (_isDown)
    {
        if (_speed > -200.0f)
        {
            _speed -= delta.count() * 460.0f;
        }
    }

    auto vector = glm::rotate(glm::vec2{ 0.0f, -1.0f }, glm::radians(get_rotation()));
    
    if (_speed > 0.1f)
    {
        _pos += vector * delta.count() * _speed;
        _speed -= delta.count() * 400.0f;
        _transform = std::nullopt;

        if (_speed <= 0.1f)
        {
            _speed = 0.0f;
        }
    }

    if (_speed < -0.1f)
    {
        _pos += vector * delta.count() * _speed;
        _speed += delta.count() * 400.0f;
        _transform = std::nullopt;

        if (_speed >= -0.1f)
        {
            _speed = 0.0f;
        }
    }
    
    _body->set_rotation(_rotation);
    
    //
    //vector = glm::rotate(_pos, _rotation);    
    //_rot = _rotation * delta.count();
     //_rot = _rotation;
    //set_anchor(vector);
    //_transform = std::nullopt;

}
