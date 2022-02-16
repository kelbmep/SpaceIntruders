#pragma once

#include <UIManager.hpp>

class Engine;
class UserData;

class UI
{
public:
	UI(const Engine&, UserData&);

	void update();
private:
	const Engine& _engine;
	UserData& _userdata;

	std::shared_ptr<BeginItem> _begin;
	std::shared_ptr<Menu::Text> _text;
	std::shared_ptr<Menu::ProgressBar> _progress_bar;
	std::shared_ptr<Menu::Text> _coins;
	std::shared_ptr<EndItem> _end;
};