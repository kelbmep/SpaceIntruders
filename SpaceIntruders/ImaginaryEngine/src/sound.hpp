#pragma once

#include <string>
#include <SDL.h>
#include <memory>

class Sound 
{
public:
	explicit Sound(std::string, bool, float);
	~Sound();

	void play();
	void pause();
	void stop();
	
	bool is_playing();

	enum class State {
		Play,
		Pause,
		Stop
	};

	State _state = State::Pause;

	friend class AudioManager;
private:
	size_t _file_len = 0;
	size_t _pos = 0;
	
	uint8_t* _data = nullptr;

	SDL_AudioSpec _audio_spec_from_file{};
	SDL_AudioSpec get_AudioFormat() const;
	
	bool _isLoop = false;

	float _volume = 0.5;
};