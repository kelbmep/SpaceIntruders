#include <button.hpp>
#include <engine.hpp>
#include <sprite.hpp>

Button::Button(const Engine& engine, std::string file_name, float x, float y, bool align, float scale) : Node(engine), _scale(scale)
{
	_button = std::make_shared<Sprite>(engine, file_name);
	_button->set_scale(glm::vec2(scale));
	if (align)
	{
		glm::vec2 button_size = -(_button->get_transform() * glm::vec3(1.0f)).xy();
		_button->set_position(glm::vec2(x - button_size.x, y + button_size.y));
	}
	else
	{
		_button->set_position(glm::vec2(x, y));
	}
	this->add_node(_button);
	engine.get_event_manager().add_delegate(this);
}

void Button::handle_event(MouseEvent e)
{
	glm::vec2 button_size = glm::vec2(_engine.get_window_width() / _engine.get_virtual_resolution().x * _button->get_size().x * _button->get_scale().x,
		_engine.get_window_height() / _engine.get_virtual_resolution().y * _button->get_size().y * _button->get_scale().y);
	glm::vec2 button_pos = glm::vec2(_engine.get_window_width() / _engine.get_virtual_resolution().x * _button->get_position().x,
		_engine.get_window_height() / _engine.get_virtual_resolution().y * _button->get_position().y);
	if (e.x > button_pos.x - button_size.x / 2
		&& e.x < button_pos.x + button_size.x / 2
		&& e.y > button_pos.y - button_size.y / 2
		&& e.y < button_pos.y + button_size.y / 2
		&& e.type == KeyType::KeyDown
		&& e.button == MouseButton::Left)
	{
		_button->set_scale(glm::vec2(0.8f * _scale));
	}
	else if (_button->get_scale() == glm::vec2(0.8f * _scale)
			&& e.type == KeyType::KeyUp
			&& e.button == MouseButton::Left)
	{
		_state = !_state;
		_button->set_scale(glm::vec2(_scale));
	}
}

void Button::visitSelf()
{
}

bool Button::get_state() const
{
	return _state;
}