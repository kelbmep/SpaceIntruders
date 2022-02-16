#pragma once

#include <UIManager.hpp>
#include <node.hpp>
#include <memory>

class Engine;
class UserData;

enum class MenuState
{
	None,
	ShowMainMenu,
	ShowSettingsMenu,
	ShowControlsMenu,
	HideMainMenu,
	HideSettingsMenu,
	HideControlsMenu,
	MainMenuShown,
	SettingsMenuShown,
	ControlsMenuShown,
	Unknown
};

class MainMenu : public EventManager::Delegate
{
public:
	MainMenu(const Engine&, UserData&);

	void handle_event(KeyEvent) override;

	void add_main_layout();
	void remove_main_layout();

	void add_settings_layout();
	void remove_settings_layout();

	void add_controls_layout();
	void remove_controls_layout();

	void show_main_menu();
	void hide_main_menu();

	void show_settings_menu();
	void hide_settings_menu();

	void show_controls_menu();
	void hide_controls_menu();

	void show_main_menu_from_settings();

	int get_volume() const;

	void update();
private:
	const Engine& _engine;
	UserData& _userdata;

	MenuState _menu_state = MenuState::None;

	int _volume = 60;
	
	std::shared_ptr<BeginItem> _main_menu;
	std::shared_ptr<Menu::Button> _play;
	std::shared_ptr<Menu::Button> _save;
	std::shared_ptr<Menu::Button> _settings;
	std::shared_ptr<Menu::Button> _controls;
	std::shared_ptr<Menu::Button> _quit;
	std::shared_ptr<BeginItem> _settings_menu;
	std::shared_ptr<Menu::Slider> _slider;
	std::shared_ptr<Menu::Button> _back;
	std::shared_ptr<BeginItem> _controls_menu;
	std::shared_ptr<Menu::Text> _controls_text;
	std::shared_ptr<EndItem> _end_menu;
};