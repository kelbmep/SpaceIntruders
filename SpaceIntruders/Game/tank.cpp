#include "tank.hpp"
#include "bullet.hpp"
#include <glm/gtx/rotate_vector.hpp>
#include <../src/audioManager.hpp>
#include <../src/engine.hpp>
#include <../src/sprite.hpp>
#include <../src/node.hpp>

Tank::Tank(const Engine& engine) : _engine{ engine }
{
    _shot = _engine.get_audio_manager().create_sound("../../../../SpaceIntruders/ImaginaryEngine/src/sound/Shot.wav", false, 0.6f);
    _movement = _engine.get_audio_manager().create_sound("../../../../SpaceIntruders/ImaginaryEngine/src/sound/Movement.wav", true, 0.7f);

    _body = std::make_shared<Sprite>(_engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_body.png");
    _tower = std::make_shared<Sprite>(_engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/tank_tower.png");
    this->set_position(glm::vec2(_engine.get_window_width() * 0.5f,
                                 _engine.get_window_height() * 0.5f));

    _body->set_scale(glm::vec2(0.4f, 0.4f));

    auto &tank_body_size = _body->get_size();
    
    _tower->set_position(glm::vec2(tank_body_size.x * 0.5f, tank_body_size.y * 0.539f));
    _tower->set_anchor(glm::vec2(0.5f, 0.694f));

    this->add_node(_body);
    _body->add_node(_tower);

    _prev_time = std::chrono::high_resolution_clock::now();
}

void Tank::handle_event(KeyEvent e)
{
    if (e.key == KeyCode::UP)
    {
        _isUp = (e.type == KeyType::KeyDown);
    }

    if (e.key == KeyCode::DOWN)
    {
        _isDown = (e.type == KeyType::KeyDown);
    }

    if (e.key == KeyCode::LEFT)
    {
        _isLeft = (e.type == KeyType::KeyDown);
    }

    if (e.key == KeyCode::RIGHT)
    {
        _isRight = (e.type == KeyType::KeyDown);
    }

    if (e.key == KeyCode::A)
    {
        _isA = (e.type == KeyType::KeyDown);
    }

    if (e.key == KeyCode::D)
    {
        _isD = (e.type == KeyType::KeyDown);
    }

    if (e.key == KeyCode::SPACE && e.type == KeyType::KeyUp)
    {
        auto vector = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(get_rotation() + _tower->get_rotation()));
        auto vector2 = glm::rotate(glm::vec2(0.0f, -1.0f), glm::radians(get_rotation()));
        
        auto bullet = std::make_shared<Bullet>(_engine, vector + vector2 * _speed / 1500.0f);
        bullet->set_position(get_position() + vector * 60.0f);
        _engine.get_scene()->add_node(std::move(bullet));
        
        _shot->stop();
        _shot->play();
    }
}

void Tank::visitSelf()
{
    auto now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> delta = now - _prev_time;
    _prev_time = now;

    if (_isA) 
    {
        _tower->set_rotation(_tower->get_rotation() - 0.3f);
    }
    if (_isD) 
    {
        _tower->set_rotation(_tower->get_rotation() + 0.3f);
    }

    if (_isLeft)
    {
        if (std::abs(_turn_speed) < 70.0f)
        {
            if (_speed >= 0)
            {
                _turn_speed += delta.count() * 120.0f + _speed * 0.002f;
            }
            else
            {
                _turn_speed += delta.count() * 120.0f - _speed * 0.002f;
            }
        }

        _transform = std::nullopt;
    }
    if (_isRight)
    {
        if (std::abs(_turn_speed) < 70.0f)
        {
            if (_speed >= 0)
            {
                _turn_speed -= delta.count() * 120.0f + _speed * 0.002f;
            }
            else
            {
                _turn_speed -= delta.count() * 120.0f - _speed * 0.002f;
            }
        }

        _transform = std::nullopt;
    }

    if (_turn_speed > 0.1f)
    {
        set_rotation(get_rotation() - delta.count() * _turn_speed);
        _turn_speed -= delta.count() * 115.0f;
        
        if (_turn_speed <= 0.1f)
        {
            _turn_speed = 0.0f;
        }
    }
    else if (_turn_speed < -0.1f)
    {
        set_rotation(get_rotation() - delta.count() * _turn_speed);
        _turn_speed += delta.count() * 115.0f;
        
        if (_turn_speed >= -0.1f)
        {
            _turn_speed = 0.0f;
        }
    }

    if (_isUp)
    {
        if (_speed < 400.0f)
        {
            _speed += delta.count() * 400.0f;
        }
    }

    if (_isDown)
    {
        if (_speed > -160.0f)
        {
            _speed -= delta.count() * 400.0f;
        }
    }

    auto vector = glm::rotate(glm::vec2{ 0.0f, -1.0f }, glm::radians(get_rotation()));
    
    if (_speed > 0.1f)
    {
        set_position(get_position() + vector * delta.count() * _speed);
        _speed -= delta.count() * 350.0f;
        
        if (_speed <= 0.1f)
        {
            _speed = 0.0f;
        }
    }

    if (_speed < -0.1f)
    {
        set_position(get_position() + vector * delta.count() * _speed);
        _speed += delta.count() * 350.0f;
        
        if (_speed >= -0.1f)
        {
            _speed = 0.0f;
        }
    }
    
    if (std::abs(_speed) > 40.0f || std::abs(_turn_speed) > 10.0f)
    {
        _movement->play();
    }
    else
    {
        _movement->pause();
    }
    
    auto tank_length = _body.get()->get_scale().y * _body.get()->get_size().y;
    auto window_width = _engine.get_window_width(), window_height = _engine.get_window_height();
    auto position_x = get_position().x, position_y = get_position().y;

    if (position_x < -0.6f * tank_length)
        set_position(get_position() + glm::vec2{ window_width, 0 } + 1.2f * glm::vec2{ tank_length, 0 });
    else if (position_x > window_width + 0.6f * tank_length)
        set_position(get_position() - glm::vec2{ window_width, 0 } - 1.2f * glm::vec2{ tank_length, 0 });
    
    if (position_y < -0.6f * tank_length)
        set_position(get_position() + glm::vec2{ 0, window_height } + 1.2f * glm::vec2{ 0, tank_length });
    else if (position_y > window_height + 0.6f * tank_length)
        set_position(get_position() - glm::vec2{ 0, window_height } - 1.2f * glm::vec2{ 0, tank_length });
}

Tank::~Tank()
{
    _shot->stop();
    _movement->stop();
}