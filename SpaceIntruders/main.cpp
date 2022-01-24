#include <engine.hpp>
#include <../../Game/tank.hpp>
#include <tiny_obj_loader.cc>
#include <../../Game/bullet.hpp>
#include <button.hpp>
#include <playlist.hpp>
#include <UIManager.hpp>
#include <physicalSprite.hpp>

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

	{
		auto box_sprite = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/res/box.png");
		box_sprite->set_scale(20.0f / box_sprite->get_size());

		b2PolygonShape box;
		box.SetAsBox(10.0f, 10.0f);

		b2Body* body = NULL;
		b2BodyDef bd;
		bd.type = b2_dynamicBody;

		bd.position.Set(5.0f, 115.0f);
		body = (&engine.get_world())->CreateBody(&bd);
		body->CreateFixture(&box, 0.5f);

		body->SetLinearVelocity(b2Vec2(200.0f, 200.0f));

		auto physical_box = std::make_shared<PhysicalSprite>(engine, body, box_sprite);

		b2PolygonShape box2;
		box2.SetAsBox(50.0f, 50.0f);

		b2Body* body2 = NULL;
		b2BodyDef bd2;
		bd2.type = b2_dynamicBody;

		bd2.position.Set(300.0f, 300.0f);
		body2 = (&engine.get_world())->CreateBody(&bd2);
		body2->CreateFixture(&box2, 0.5f);

		body2->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

		auto box_sprite2 = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/res/box.png");
		box_sprite2->set_scale(100.0f / box_sprite2->get_size());

		auto physical_box2 = std::make_shared<PhysicalSprite>(engine, body2, box_sprite2);

		b2Body* body3 = NULL;
		b2BodyDef bd3;
		bd3.type = b2_dynamicBody;

		bd3.position.Set(600.0f, 485.0f);
		body3 = (&engine.get_world())->CreateBody(&bd3);
		body3->CreateFixture(&box, 0.5f);

		body3->SetLinearVelocity(b2Vec2(-200.0f, -200.0f));

		auto box_sprite3 = std::make_shared<Sprite>(engine, "../../../../SpaceIntruders/ImaginaryEngine/res/box.png");
		box_sprite3->set_scale(20.0f / box_sprite3->get_size());

		auto physical_box3 = std::make_shared<PhysicalSprite>(engine, body3, box_sprite3);

		engine.get_scene()->add_node(physical_box);
		engine.get_scene()->add_node(physical_box2);
		engine.get_scene()->add_node(physical_box3);
	}

	auto tank = std::make_shared<Tank>(engine);

	engine.get_event_manager().add_delegate(tank.get());
	engine.get_scene()->add_node(tank);

	auto pause_button = std::make_shared<Button>(engine, "../../../../SpaceIntruders/ImaginaryEngine/res/pause.png");
	auto play_button = std::make_shared<Button>(engine, "../../../../SpaceIntruders/ImaginaryEngine/res/play.png");
	
	engine.get_scene()->add_node(play_button);

	engine.get_playlist()->add_track("../../../../SpaceIntruders/ImaginaryEngine/res/track1.wav");
	engine.get_playlist()->add_track("../../../../SpaceIntruders/ImaginaryEngine/res/track2.wav");
	engine.get_playlist()->add_track("../../../../SpaceIntruders/ImaginaryEngine/res/track3.wav");

	engine.get_event_manager().add_delegate(engine.get_playlist().get());

	int volume = 60;
	
	auto main_menu = std::make_shared<BeginItem>("Main menu");
	auto play = std::make_shared<Menu::Button>("Play");
	auto settings = std::make_shared<Menu::Button>("Settings");
	auto quit = std::make_shared<Menu::Button>("Quit");
	auto settings_menu = std::make_shared<BeginItem>("Settings");
	auto slider = std::make_shared<Menu::Slider>("Volume", &volume, 0, 100);
	auto back = std::make_shared<Menu::Button>("<-- Back");
	auto end_menu = std::make_shared<EndItem>();
	
	engine.get_ui_manager()->add_menu_item(main_menu);
	engine.get_ui_manager()->add_menu_item(play);
	engine.get_ui_manager()->add_menu_item(settings);
	engine.get_ui_manager()->add_menu_item(quit);
	engine.get_ui_manager()->add_menu_item(end_menu);

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
			engine.get_scene()->add_node(play_button, -1);
			engine.get_scene()->remove_node(pause_button);
			is_play = !is_play;
			engine.get_playlist()->pause();
		}

		if ((!engine.get_ui_manager()->get_show_main_menu() && is_settings) || back->get_is_clicked())
		{
			engine.get_ui_manager()->remove_menu_item(settings_menu);
			engine.get_ui_manager()->remove_menu_item(slider);
			engine.get_ui_manager()->remove_menu_item(back);
			engine.get_ui_manager()->remove_menu_item(end_menu);
			engine.get_ui_manager()->add_menu_item(main_menu);
			engine.get_ui_manager()->add_menu_item(play);
			engine.get_ui_manager()->add_menu_item(settings);
			engine.get_ui_manager()->add_menu_item(quit);
			engine.get_ui_manager()->add_menu_item(end_menu);
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
			engine.get_ui_manager()->remove_menu_item(main_menu);
			engine.get_ui_manager()->remove_menu_item(play);
			engine.get_ui_manager()->remove_menu_item(settings);
			engine.get_ui_manager()->remove_menu_item(quit);
			engine.get_ui_manager()->remove_menu_item(end_menu);
			engine.get_ui_manager()->add_menu_item(settings_menu);
			engine.get_ui_manager()->add_menu_item(slider);
			engine.get_ui_manager()->add_menu_item(back);
			engine.get_ui_manager()->add_menu_item(end_menu);
			settings->set_is_clicked(false);
			is_settings = true;
		}

		engine.update();
		engine.get_playlist()->set_volume(volume);
	}
	return 0;
}