#include <button.hpp>
#include <engine.hpp>
#include <sprite.hpp>
#include <iostream>

Button::Button(const Engine& engine, std::string file_name)
{
	_button = std::make_shared<Sprite>(engine, file_name);
	_button->set_scale(glm::vec2(0.1f));
	glm::vec2 button_size = -(_button->get_transform() * glm::vec3(1.0f)).xy();
	_button->set_position(glm::vec2(engine.get_window_width() * 0.99f - button_size.x, engine.get_window_height() * 0.01f + button_size.y));
	this->add_node(_button);
	engine.get_event_manager().add_delegate(this);
}

void Button::handle_event(EventManager::MouseEvent e)
{
	glm::vec2 button_size = _button->get_size() * _button->get_scale();
	glm::vec2 button_pos = _button->get_position();
	if (e.x > button_pos.x - button_size.x / 2
		&& e.x < button_pos.x + button_size.x / 2
		&& e.y > button_pos.y - button_size.y / 2
		&& e.y < button_pos.y + button_size.y / 2
		&& e.type == EventManager::KeyType::KeyDown
		&& e.button == EventManager::MouseButton::Left)
	{
		_button->set_scale(glm::vec2(0.08f));
	}
	else if (_button->get_scale() == glm::vec2(0.08f)
			&& e.type == EventManager::KeyType::KeyUp
			&& e.button == EventManager::MouseButton::Left)
	{
		_state = !_state;
		_button->set_scale(glm::vec2(0.1f));
	}
}

void Button::visitSelf()
{

}

bool Button::get_state() const
{
	return _state;
}