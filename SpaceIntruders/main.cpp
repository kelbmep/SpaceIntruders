// SpaceIntruders.cpp: определяет точку входа для приложения.
//
#include "ImaginaryEngine/src/engine.hpp"

using namespace std;

int main()
{
	Engine engine{};
	engine.init("Mace Windows");
	const std::string inputfile = "../../../../SpaceIntruders/ImaginaryEngine/src/african_head.obj";
	engine.drawObj(inputfile, 800, 800);
	while (engine.isActive())
	{
		engine.update();
	}
	return 0;
}
