#include <engine.hpp>
#include <audioManager.hpp>
#include <../../Game/tank.hpp>
#include <tiny_obj_loader.cc>
#include <../../Game/bullet.hpp>
#include <button.hpp>

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

	std::shared_ptr<Button> pause = std::make_shared<Button>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/pause.png");
	std::shared_ptr<Button> play = std::make_shared<Button>(engine, "../../../../SpaceIntruders/ImaginaryEngine/src/img/play.png");
	
	engine.get_scene()->add_node(play);
	play->set_zOrder(-1);

	auto background = engine.get_audio_manager().create_sound("../../../../SpaceIntruders/ImaginaryEngine/src/sound/Background.wav", true, 0.8f);
	background->play();
	
	bool is_play = false;
	while (engine.isActive()) 
	{
		if (!pause->get_state() && !is_play)
		{
			engine.get_scene()->remove_node(play);
			engine.get_scene()->add_node(pause);
			is_play = !is_play;
			background->play();
		}
		else if (pause->get_state() && is_play)
		{
			engine.get_scene()->add_node(play);
			engine.get_scene()->remove_node(pause);
			is_play = !is_play;
			background->pause();
		}
		engine.update();
	}
	return 0;
}
