#include <sound.hpp>
#include <stdexcept>

Sound::Sound(std::string file_name, bool is_loop, float volume): _is_loop(is_loop)
{
	auto* file = SDL_RWFromFile(file_name.c_str(), "rb");
	
	if (file == nullptr)
	{
		throw std::runtime_error("Cannot open file: " + file_name);
	}
	
	if (volume < 0)
	{
		_volume = 0.0;
	}
	else if (volume > 1)
	{
		_volume = 1.0;
	}
	else
	{
		_volume = volume;
	}

	uint32_t audio_len = 0;

	auto* audio_spec = SDL_LoadWAV_RW(file, 1, &_audio_spec_from_file, &_data, &audio_len);
	
	_file_len = audio_len;
	
	if (audio_spec == nullptr)
	{
		throw std::runtime_error("Sound isn't loaded.");
	}

	_state = State::Pause;
}

void Sound::play()
{
	_state = State::Play;
}

void Sound::pause()
{
	_state = State::Pause;
}

void Sound::stop()
{
	_state = State::Stop;
	_pos = 0;
}

bool Sound::is_playing()
{
	return _state == State::Play;
}

SDL_AudioSpec Sound::get_audio_format() const
{
	return _audio_spec_from_file;
}

float Sound::get_volume() const
{
	return _volume;
}

void Sound::set_volume(float vol)
{
	if (vol < 0)
	{
		_volume = 0.0;
	}
	else if (vol > 1)
	{
		_volume = 1.0;
	}
	else
	{
		_volume = vol;
	}
}

Sound::~Sound()
{
	SDL_FreeWAV(_data);
}