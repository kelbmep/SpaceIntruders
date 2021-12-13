#include <sound.hpp>
#include <stdexcept>
#include <vector>

Sound::Sound(std::string file_name, bool is_loop, float volume): _isLoop(is_loop)
{
	SDL_RWops* file = SDL_RWFromFile(file_name.data(), "rb");
	
	if (file == nullptr)
	{
		throw std::runtime_error("Cannot open file: " + std::string(file_name));
	}
	
	if (volume < 0)
		_volume = 0.0;
	else if (volume > 1)
		_volume = 1.0;

	uint32_t audio_len = 0;

	SDL_AudioSpec* audio_spec = SDL_LoadWAV_RW(file, 1, &_audio_spec_from_file, _data.get(), &audio_len);
	
	_file_len = audio_len;
	
	if (audio_spec == nullptr)
	{
		throw std::runtime_error("Sound isn't loaded.");
	}

	_state = State::STOP;
}

void Sound::play()
{
	_state = State::PLAY;
}

void Sound::pause()
{
	_state = State::PAUSE;
}

void Sound::stop()
{
	_state = State::STOP;
	_pos = 0;
}

bool Sound::is_playing()
{
	return _state == State::PLAY;
}

SDL_AudioSpec Sound::get_AudioFormat() const
{
	return _audio_spec_from_file;
}

Sound::~Sound()
{
	SDL_FreeWAV(*_data.get());
}