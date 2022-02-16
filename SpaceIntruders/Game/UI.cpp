#include "UI.hpp"
#include "userData.hpp"
#include <engine.hpp>

UI::UI(const Engine& engine, UserData& userdata) : _engine(engine), _userdata(userdata)
{
	_begin = std::make_shared<BeginItem>("Fuel", ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove);
	_text = std::make_shared<Menu::Text>("Fuel");
	_progress_bar = std::make_shared<Menu::ProgressBar>(_userdata.get_fuel());
	_coins = std::make_shared<Menu::Text>("0 coins");
	_end = std::make_shared<EndItem>();
	_engine.get_ui_manager()->add_menu_item(_begin);
	_engine.get_ui_manager()->add_menu_item(_text);
	_engine.get_ui_manager()->add_menu_item(_progress_bar);
	_engine.get_ui_manager()->add_menu_item(_coins);
	_engine.get_ui_manager()->add_menu_item(_end);
}

void UI::update()
{
	_progress_bar->set_progress(_userdata.get_fuel());
	_coins->set_text(std::to_string(_userdata.get_coins()) + " coins");
}