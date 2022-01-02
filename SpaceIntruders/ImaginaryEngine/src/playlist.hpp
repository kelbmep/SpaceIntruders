#pragma once

#include <string>
#include <vector>
#include <memory>
#include <eventManager.hpp>

class Sound;
class Engine;

class Playlist : public EventManager::Delegate 
{
public:
	explicit Playlist(const Engine&);
	
	void add_track(std::string);
	
	void play();
	void pause();
	
	void play_previous();
	void play_next();
	
	void update();
	
	void handle_event(KeyEvent) override;

	void set_volume(int);
private:
	const Engine& _engine;
	
	std::shared_ptr<Sound> _current_song;
	mutable std::vector<std::shared_ptr<Sound>> _songs;
	
	bool _next = false;
	bool _previous = false;
};