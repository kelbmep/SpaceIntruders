#include "tank.hpp"
#include <iostream>
//#include "bullet.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <vector>

Tank::Tank(const Engine& engine, std::shared_ptr<AudioManager> audioManager) : _engine{ engine }
{
    _audioManager = audioManager;
    _body = std::make_shared<Sprite>(_engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_body.png");
    _tower = std::make_shared<Sprite>(_engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_tower.png");
    this->set_position(glm::vec2(_engine.get_window_width() * 0.5f,
                                 _engine.get_window_height() * 0.5f));

    _body->set_scale(glm::vec2(0.4f, 0.4f));

    glm::vec2 tank_body_size = _body->get_size();
    
    _tower->set_position(glm::vec2(tank_body_size.x * 0.5f, tank_body_size.y * 0.539f));
    _tower->set_anchor(glm::vec2(0.5f, 0.694f));

    this->addNode(_body);
    _body->addNode(_tower);
}

void Tank::handle_event(EventManager::QuitEvent)
{
    std::cout << "Iquit";
}

void Tank::handle_event(EventManager::KeyEvent e)
{
    if (e.key == EventManager::KeyCode::Up)
    {
        _isUp = (e.type == EventManager::KeyType::KeyDown);
    }

    if (e.key == EventManager::KeyCode::Down)
    {
        _isDown = (e.type == EventManager::KeyType::KeyDown);
    }

    if (e.key == EventManager::KeyCode::Left)
    {
        _isLeft = (e.type == EventManager::KeyType::KeyDown);
    }

    if (e.key == EventManager::KeyCode::Right)
    {
        _isRight = (e.type == EventManager::KeyType::KeyDown);
    }

    if (e.key == EventManager::KeyCode::A)
    {
        _isA = (e.type == EventManager::KeyType::KeyDown);
    }

    if (e.key == EventManager::KeyCode::D)
    {
        _isD = (e.type == EventManager::KeyType::KeyDown);
    }

    if (e.key == EventManager::KeyCode::Space && e.type == EventManager::KeyType::KeyUp)
    {
        auto sound = _audioManager->createSound("../../../../SpaceIntruders/ImaginaryEngine/src/sound/Shot.wav", false, 0.05);
        sound->play();
        _shots.push_back(sound);
        //auto vector = glm::rotate(glm::vec2( 0.0f, -1.0f ), glm::radians(get_rotation()));
        //auto shot = std::make_shared<Bullet>(_engine, vector * _speed);
        //shot->set_position(_pos * vector);// *160.0f);// _pos* vector * 160.0f);
        //this->addNode(shot);
        std::cout << "Pew" << std::endl;
    }
}

void Tank::visitSelf()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - _prev_time;
    _prev_time = now;

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
            if (_speed >= 0)
            {
                _turnSpeed += delta.count() * 120.0f + _speed * 0.002f;
            }
            else
            {
                _turnSpeed += delta.count() * 120.0f - _speed * 0.002f;
            }
        }

        _transform = std::nullopt;
    }
    if (_isRight)
    {
        if (std::abs(_turnSpeed) < 70.0f)
        {
            if (_speed >= 0)
            {
                _turnSpeed -= delta.count() * 120.0f + _speed * 0.002f;
            }
            else
            {
                _turnSpeed -= delta.count() * 120.0f - _speed * 0.002f;
            }
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
    else if (_turnSpeed < -0.1f)
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
    
    set_rotation(_rotation);
}
