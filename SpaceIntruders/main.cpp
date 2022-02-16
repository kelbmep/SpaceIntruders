#include <engine.hpp>
#include <../../Game/tank.hpp>
#include <../../Game/bullet.hpp>
#include <button.hpp>
#include <playlist.hpp>
#include <physicalSprite.hpp>
#include <fileManager.hpp>
#include <scheduleManager.hpp>
#include <../../Game/car.hpp>
#include <../../Game/mainMenu.hpp>
#include <../../Game/coin.hpp>
#include <../../Game/fuel.hpp>
#include <../../Game/userdata.hpp>
#include <../../Game/UI.hpp>
#include <../../Game/ground.hpp>

const auto SCREEN_WIDTH = 1280;
const auto SCREEN_HEIGHT = 720;

int main(int argc, char* argv[])
{
	Engine engine{ };

	auto mode = 0;
	if (argc > 1)
	{
		mode = std::stoi(argv[1]);
	}

	engine.get_file_manager().set_find_paths({ "SpaceIntruders/ImaginaryEngine/res/" });

	engine.init("Bumpy Road Racing v0.1", SCREEN_WIDTH, SCREEN_HEIGHT, mode);

	engine.set_virtual_resolution(glm::vec2{ 1920.0f, 1080.0f });

	auto start_screen = std::make_shared<Sprite>(engine, "start screen.jpg");
	start_screen->set_scale(glm::vec2(engine.get_virtual_resolution().x / SCREEN_WIDTH, engine.get_virtual_resolution().y / SCREEN_HEIGHT));

	auto play_game = std::make_shared<Button>(engine, "play_text.png", engine.get_virtual_resolution().x / 2.0f, engine.get_virtual_resolution().y / 2.0f);
	auto exit_game = std::make_shared<Button>(engine, "exit_text.png", engine.get_virtual_resolution().x / 2.0f, engine.get_virtual_resolution().y * 4.0f / 5.0f, false, 0.9f);

	engine.get_scene()->add_node(start_screen);
	engine.get_camera()->add_node(play_game);
	engine.get_camera()->add_node(exit_game);
	engine.get_scene()->add_node(engine.get_camera());

	while (engine.is_disabled())
	{
		engine.update();
		if (play_game->get_state())
		{
			engine.enable();
			engine.get_camera()->remove_node(start_screen);
			engine.get_camera()->remove_node(play_game);
			engine.get_camera()->remove_node(exit_game);
		}
		else if (exit_game->get_state())
		{
			return 0;
		}
	}

	UserData user_data(engine);

	float ground_level = SCREEN_HEIGHT - 100.0f;
	auto ground = std::make_unique<Ground>(engine, ground_level);

	auto car = std::make_shared<Car>(engine, user_data, 20.0f, ground_level - 8.0f);

	engine.get_event_manager().add_delegate(car.get());
	engine.get_scene()->add_node(car);

	engine.get_scene()->add_node(std::make_shared<Fuel>(engine, user_data, 340.0f, ground_level - 1.5f));

	float x1 = SCREEN_WIDTH / 2 - 150.0f;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			engine.get_scene()->add_node(std::make_shared<Coin>(engine, user_data, x1 + i * 3, ground_level - 1.5f));
		}
		x1 += 158.0f;
	}

	auto pause_button = std::make_shared<Button>(engine, "pause.png", engine.get_virtual_resolution().x * 0.99f, engine.get_virtual_resolution().y * 0.01f, true, 0.2f);
	auto play_button = std::make_shared<Button>(engine, "play.png", engine.get_virtual_resolution().x * 0.99f, engine.get_virtual_resolution().y * 0.01f, true, 0.2f);
	
	engine.get_camera()->add_node(play_button);

	auto main_menu = std::make_unique<MainMenu>(engine, user_data);

	engine.get_playlist()->add_track("track1.wav");
	engine.get_playlist()->add_track("track2.wav");
	engine.get_playlist()->add_track("track3.wav");

	engine.get_event_manager().add_delegate(engine.get_playlist().get());

	engine.get_playlist()->play();
	bool is_play = false;
	while (engine.is_active()) 
	{
		if (!pause_button->get_state() && !is_play)
		{
			engine.get_camera()->remove_node(play_button);
			engine.get_camera()->add_node(pause_button);
			is_play = !is_play;
			engine.get_playlist()->play();
		}
		else if (pause_button->get_state() && is_play)
		{
			engine.get_camera()->add_node(play_button);
			engine.get_camera()->remove_node(pause_button);
			is_play = !is_play;
			engine.get_playlist()->pause();
		}

		auto delta = glm::vec2(car->get_car_body()->get_position().x - engine.get_camera()->get_position().x + 20.0f, 
							   car->get_car_body()->get_position().y - engine.get_camera()->get_position().y);
		engine.get_camera()->set_position(engine.get_camera()->get_position() + delta * 40.0f * engine.get_schedule_manager().get_delta().count());
		engine.get_camera()->set_scale(glm::vec2(0.05f));

		engine.update();
		main_menu->update();
		user_data.update();
	}
	return 0;
}