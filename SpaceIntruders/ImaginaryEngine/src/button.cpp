#include <button.hpp>
#include <engine.hpp>
#include <sprite.hpp>

Button::Button(const Engine& engine, std::string file_name)
{
	_button = std::make_shared<Sprite>(engine, file_name);
	this->set_scale(glm::vec2(0.1f));
	glm::vec2 button_size = _button->get_size() * this->get_scale();
	this->set_position(glm::vec2(engine.get_window_width() * 0.99f - button_size.x / 2, engine.get_window_height() * 0.01f + button_size.y / 2));
	this->add_node(_button);
	engine.get_event_manager().add_delegate(this);
}

void Button::handle_event(EventManager::MouseEvent e)
{
	glm::vec2 button_size = _button->get_size();
	glm::vec2 button_pos = get_position();
	if (e.x > button_pos.x - button_size.x / 2
		&& e.x < button_pos.x + button_size.x / 2
		&& e.y > button_pos.y - button_size.y / 2
		&& e.y < button_pos.y + button_size.y / 2
		&& e.type == EventManager::KeyType::KeyDown)
	{
		this->set_scale(glm::vec2(0.08f));
	}
	else
	{
		this->set_scale(glm::vec2(0.1f));
		_state = !_state;
	}
}

void Button::visitSelf()
{

}

bool Button::get_state() const
{
	return _state;
}