#include "userData.hpp"
#include "UI.hpp"
#include <engine.hpp>
#include <fileManager.hpp>
#include <audioManager.hpp>
#include <sound.hpp>
#include <fstream>

UserData::UserData(const Engine& engine)// : _engine(engine)
{
	_coin_sound = engine.get_audio_manager().create_sound("coin.wav", false, 1.0f);

	std::ifstream fin("UserData.txt");
	//std::string data, line;

	if (fin.fail())
	{
		_coins = 0;
	}
	else
	{
		fin >> _coins;
		fin.close();
	}

	_UI = std::make_unique<UI>(engine, *this);
}

float UserData::get_fuel() const
{
	return _fuel;
}

void UserData::set_fuel(float fuel)
{
	_fuel = fuel;
}

float UserData::get_rate() const
{
	return _fuel_rate;
}

void UserData::set_rate(float rate)
{
	_fuel_rate = rate;
}

int UserData::get_coins() const
{
	return _coins;
}

void UserData::set_coins(int coins)
{
	_coins = coins;
}

std::shared_ptr<Sound> UserData::get_coin_sound() const
{
	return _coin_sound;
}

void UserData::update()
{
	_fuel -= _fuel_rate / 100000.0f;
	_UI->update();
}