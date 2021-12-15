#include <engine.hpp>
#include <sprite.hpp>
#include <render.hpp>
#include <window.hpp>
#include <../../Game/tank.hpp>
#include <chrono>
#include <tiny_obj_loader.cc>
#include <sound.hpp>
#include <audioManager.hpp>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

int main(int argc, char* argv[])
{
	Engine engine{ };
	
	int mode = 0;
	if (argc > 1)
	{
		mode = std::stoi(argv[1]);
	}

	engine.init("Mace Windows", SCREEN_WIDTH, SCREEN_HEIGHT, mode);
	
	std::shared_ptr<Tank> tank = std::make_shared<Tank>(engine);

	engine.get_event_manager().add_delegate(tank.get());

	engine.get_scene()->addNode(tank);

	auto background = engine.get_audio_manager().createSound("../../../../SpaceIntruders/ImaginaryEngine/src/sound/Background.wav", true, 0.4);
	
	background->play();
	
	while (engine.isActive()) 
	{
		engine.update();
	}
	return 0;
}
