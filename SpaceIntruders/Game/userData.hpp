#pragma once

#include <memory>

class Engine;
class UI;
class Sound;

class UserData
{
public:
	UserData(const Engine&);
	
	float get_fuel() const;
	void set_fuel(float);

	float get_rate() const;
	void set_rate(float);

	int get_coins() const;
	void set_coins(int);

	std::shared_ptr<Sound> get_coin_sound() const;

	void update();
private:
	std::unique_ptr<UI> _UI;

	float _fuel = 1.0f;
	float _fuel_rate = 10.0f;

	int _coins = 0;

	std::shared_ptr<Sound> _coin_sound;
};