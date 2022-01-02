#include <engine.hpp>
#include <../../Game/tank.hpp>
#include <tiny_obj_loader.cc>
#include <../../Game/bullet.hpp>
#include <button.hpp>
#include <playlist.hpp>
#include <UIManager.hpp>

const auto SCREEN_WIDTH = 1024;
const auto SCREEN_HEIGHT = 768;

int main(int argc, char* argv[])
{
	Engine engine{ };
	
	auto mode = 0;
	if (argc > 1)
	{
		mode = std::stoi(argv[1]);
	}

	engine.init("Mace Windows", SCREEN_WIDTH, SCREEN_HEIGHT, mode);
	
	auto tank = std::make_shared<Tank>(engine);

	engine.get_event_manager().add_delegate(tank.get());
	engine.get_scene()->add_node(tank);

	std::shared_ptr<Button> pause_button = std::make_shared<Button>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/pause.png");
	std::shared_ptr<Button> play_button = std::make_shared<Button>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/play.png");
	
	engine.get_scene()->add_node(play_button);
	play_button->set_zOrder(-1);

	engine.get_playlist()->add_track("../../../../SpaceIntruders/ImaginaryEngine/src/sound/track1.wav");
	engine.get_playlist()->add_track("../../../../SpaceIntruders/ImaginaryEngine/src/sound/track2.wav");
	engine.get_playlist()->add_track("../../../../SpaceIntruders/ImaginaryEngine/src/sound/track3.wav");

	engine.get_event_manager().add_delegate(engine.get_playlist().get());

	int volume = 60;
	
	std::shared_ptr play = std::make_shared<Menu::Button>("Play");
	std::shared_ptr settings = std::make_shared<Menu::Button>("Settings");
	std::shared_ptr slider = std::make_shared<Menu::Slider>("Volume", &volume, 0, 100);
	std::shared_ptr back = std::make_shared<Menu::Button>("<-- Back");
	std::shared_ptr quit = std::make_shared<Menu::Button>("Quit");
	
	engine.get_ui_manager()->add_menu_item(std::make_shared<BeginItem>("Menu"));
	engine.get_ui_manager()->add_menu_item(play);
	engine.get_ui_manager()->add_menu_item(settings);
	engine.get_ui_manager()->add_menu_item(quit);
	engine.get_ui_manager()->add_menu_item(std::make_shared<EndItem>());

	engine.get_playlist()->play();
	bool is_play = false, is_settings = false;
	while (engine.is_active()) 
	{
		if (!pause_button->get_state() && !is_play)
		{
			engine.get_scene()->remove_node(play_button);
			engine.get_scene()->add_node(pause_button);
			is_play = !is_play;
			engine.get_playlist()->play();
		}
		else if (pause_button->get_state() && is_play)
		{
			engine.get_scene()->add_node(play_button);
			engine.get_scene()->remove_node(pause_button);
			is_play = !is_play;
			engine.get_playlist()->pause();
		}

		if ((!engine.get_ui_manager()->get_show_main_menu() && is_settings) || back->get_is_clicked())
		{
			engine.get_ui_manager()->remove_menu_item(slider);
			engine.get_ui_manager()->remove_menu_item(back);
			engine.get_ui_manager()->add_menu_item(std::make_shared<BeginItem>("Menu"));
			engine.get_ui_manager()->add_menu_item(play);
			engine.get_ui_manager()->add_menu_item(settings);
			engine.get_ui_manager()->add_menu_item(quit);
			engine.get_ui_manager()->add_menu_item(std::make_shared<EndItem>());
			back->set_is_clicked(false);
			is_settings = false;
		}

		if (quit->get_is_clicked())
		{
			engine.get_event_manager().invoke_event(QuitEvent{});
		}
		else if (play->get_is_clicked())
		{
			engine.get_event_manager().invoke_event(KeyEvent{ KeyCode::ESCAPE, KeyType::KeyUp });
			play->set_is_clicked(false);
		}
		else if (settings->get_is_clicked())
		{
			engine.get_ui_manager()->remove_menu_item(play);
			engine.get_ui_manager()->remove_menu_item(settings);
			engine.get_ui_manager()->remove_menu_item(quit);
			engine.get_ui_manager()->add_menu_item(std::make_shared<BeginItem>("Menu"));
			engine.get_ui_manager()->add_menu_item(slider);
			engine.get_ui_manager()->add_menu_item(back);
			engine.get_ui_manager()->add_menu_item(std::make_shared<EndItem>());
			settings->set_is_clicked(false);
			is_settings = true;
		}

		engine.update();
		engine.get_playlist()->set_volume(volume);
	}
	return 0;
}