#pragma once

#include <memory>
#include <render.hpp>

class Engine;
class EventManager;
class TextureUniform;
class Mat3Uniform;
class Vec2Uniform;

class MenuItem
{
public:
	virtual void visit() = 0;
};

namespace Menu{
	class Button : public MenuItem
	{
	public:
		explicit Button(std::string text)
			: _text(std::move(text))
		{}

		void visit() override;
		bool get_is_clicked();
		void set_is_clicked(bool);
	private:
		std::string _text;
		bool _is_clicked = false;
	};

	class Slider : public MenuItem
	{
	public:
		explicit Slider(std::string text, int* val, int min, int max)
			: _text(std::move(text)), _val(std::move(val)), _min(min), _max(max)
		{}

		void visit() override;
	private:
		std::string _text;
		int _min, _max;
		int* _val;
	};
}

class BeginItem : public MenuItem
{
public:
	explicit BeginItem(std::string text)
		: _text(std::move(text))
	{}

	void visit() override;
private:
	std::string _text;
};

class EndItem : public MenuItem
{
public:
	void visit() override;
};

class UIManager : public EventManager::Delegate
{
public:
	explicit UIManager(const Engine&);
	
	void visit();

	void handle_event(MouseEvent) override;
	void handle_event(MouseMoveEvent) override;
	void handle_event(KeyEvent) override;
	void handle_event(TextInputEvent) override;

	void add_menu_item(std::shared_ptr<MenuItem>);
	void remove_menu_item(const std::shared_ptr<MenuItem>&);
	
	bool get_show_main_menu();
private:
	const Engine& _engine;
	Render::Command _command;

	bool _show_demo_window = true;
	bool _show_main_menu = false;

	bool _isLeft = false;
	bool _isRight = false;
	bool _isMiddle = false;

	glm::vec2 _mouse_pos;

	std::shared_ptr<TextureUniform> _texture_uniform;
	std::shared_ptr<Mat3Uniform> _transform_uniform;
	std::shared_ptr<Vec2Uniform> _screen_size_uniform;

	std::vector<std::shared_ptr<MenuItem>> _menu_items;
};