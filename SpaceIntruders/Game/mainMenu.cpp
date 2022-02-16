#include "mainMenu.hpp"
#include "userData.hpp"
#include <playlist.hpp>
#include <fstream>

MainMenu::MainMenu(const Engine& engine, UserData& userdata) : _engine(engine), _userdata(userdata)
{
	_main_menu = std::make_shared<BeginItem>("Main menu");
	_play = std::make_shared<Menu::Button>("Play");
	_save = std::make_shared<Menu::Button>("Save");
	_settings = std::make_shared<Menu::Button>("Settings");
	_controls = std::make_shared<Menu::Button>("Controls");
	_quit = std::make_shared<Menu::Button>("Quit");
	_settings_menu = std::make_shared<BeginItem>("Settings");
	_slider = std::make_shared<Menu::Slider>("Volume", &_volume, 0, 100);
	_back = std::make_shared<Menu::Button>("<-- Back");
	_controls_menu = std::make_shared<BeginItem>("Controls");
	_controls_text = std::make_shared<Menu::Text>("9 - previous song\n0 - next song\na - move left\nd - move right\n");
	_end_menu = std::make_shared<EndItem>();

	_engine.get_event_manager().add_delegate(this);
}

void MainMenu::handle_event(KeyEvent e)
{
	if (e.key == KeyCode::ESCAPE && e.type == KeyType::KeyUp)
	{
		switch (_menu_state)
		{
		case(MenuState::None):
			_menu_state = MenuState::ShowMainMenu;
			break;
		case(MenuState::MainMenuShown):
			_menu_state = MenuState::HideMainMenu;
			break;
		case(MenuState::SettingsMenuShown):
			_menu_state = MenuState::HideSettingsMenu;
			break;
		case(MenuState::ControlsMenuShown):
			_menu_state = MenuState::HideControlsMenu;
			break;
		default:
			break;
		}
	}
}

void MainMenu::add_main_layout()
{
	_engine.get_ui_manager()->add_menu_item(_main_menu);
	_engine.get_ui_manager()->add_menu_item(_play);
	_engine.get_ui_manager()->add_menu_item(_save);
	_engine.get_ui_manager()->add_menu_item(_settings);
	_engine.get_ui_manager()->add_menu_item(_controls);
	_engine.get_ui_manager()->add_menu_item(_quit);
	_engine.get_ui_manager()->add_menu_item(_end_menu);
}

void MainMenu::remove_main_layout()
{
	_engine.get_ui_manager()->remove_menu_item(_main_menu);
	_engine.get_ui_manager()->remove_menu_item(_play);
	_engine.get_ui_manager()->remove_menu_item(_save);
	_engine.get_ui_manager()->remove_menu_item(_settings);
	_engine.get_ui_manager()->remove_menu_item(_controls);
	_engine.get_ui_manager()->remove_menu_item(_quit);
	_engine.get_ui_manager()->remove_menu_item(_end_menu);
}

void MainMenu::add_settings_layout()
{
	_engine.get_ui_manager()->add_menu_item(_settings_menu);
	_engine.get_ui_manager()->add_menu_item(_slider);
	_engine.get_ui_manager()->add_menu_item(_back);
	_engine.get_ui_manager()->add_menu_item(_end_menu);
}

void MainMenu::remove_settings_layout()
{
	_engine.get_ui_manager()->remove_menu_item(_settings_menu);
	_engine.get_ui_manager()->remove_menu_item(_slider);
	_engine.get_ui_manager()->remove_menu_item(_back);
	_engine.get_ui_manager()->remove_menu_item(_end_menu);
}

void MainMenu::add_controls_layout()
{
	_engine.get_ui_manager()->add_menu_item(_controls_menu);
	_engine.get_ui_manager()->add_menu_item(_controls_text);
	_engine.get_ui_manager()->add_menu_item(_end_menu);
}

void MainMenu::remove_controls_layout()
{
	_engine.get_ui_manager()->remove_menu_item(_controls_menu);
	_engine.get_ui_manager()->remove_menu_item(_controls_text);
	_engine.get_ui_manager()->remove_menu_item(_end_menu);
}

void MainMenu::show_main_menu()
{
	add_main_layout();
	_menu_state = MenuState::MainMenuShown;
}

void MainMenu::hide_main_menu()
{
	remove_main_layout();
	_menu_state = MenuState::None;
}

void MainMenu::show_settings_menu()
{
	remove_main_layout();
	add_settings_layout();
	_menu_state = MenuState::SettingsMenuShown;
}

void MainMenu::hide_settings_menu()
{
	remove_settings_layout();
	_menu_state = MenuState::None;
}

void MainMenu::show_controls_menu()
{
	remove_main_layout();
	add_controls_layout();
	_menu_state = MenuState::ControlsMenuShown;
}

void MainMenu::hide_controls_menu()
{
	remove_controls_layout();
	_menu_state = MenuState::None;
}

void MainMenu::show_main_menu_from_settings()
{
	remove_settings_layout();
	add_main_layout();
	_back->set_is_clicked(false);
	_menu_state = MenuState::MainMenuShown;
}

int MainMenu::get_volume() const
{
	return _volume;
}

void MainMenu::update()
{
	switch (_menu_state)
	{
	case(MenuState::ShowMainMenu):
		show_main_menu();
		break;
	case(MenuState::ShowSettingsMenu):
		show_settings_menu();
		break;
	case(MenuState::ShowControlsMenu):
		show_controls_menu();
		break;
	case(MenuState::HideMainMenu):
		hide_main_menu();
		break;
	case(MenuState::HideSettingsMenu):
		hide_settings_menu();
		break;
	case(MenuState::HideControlsMenu):
		hide_controls_menu();
		break;
	default:
		break;
	}
	
	if (_back->get_is_clicked())
	{
		show_main_menu_from_settings();
	}
	
	if (_play->get_is_clicked())
	{
		_engine.get_event_manager().invoke_event(KeyEvent{ KeyCode::ESCAPE, KeyType::KeyUp });
		_play->set_is_clicked(false);
	}
	else if (_save->get_is_clicked())
	{
		std::ofstream fout("UserData.txt");
		fout << _userdata.get_coins();
		fout.close();
		_save->set_is_clicked(false);
	}
	else if (_settings->get_is_clicked())
	{
		_menu_state = MenuState::ShowSettingsMenu;
		_settings->set_is_clicked(false);
	}
	else if (_controls->get_is_clicked())
	{
		_menu_state = MenuState::ShowControlsMenu;
		_controls->set_is_clicked(false);
	}
	else if (_quit->get_is_clicked())
	{
		_engine.get_event_manager().invoke_event(QuitEvent{});
	}

	_engine.get_playlist()->set_volume(this->get_volume());
}