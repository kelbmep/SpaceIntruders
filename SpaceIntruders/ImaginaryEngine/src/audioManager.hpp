#pragma once

#include <sound.hpp>
#include <vector>

class Engine;

class AudioManager 
{
public:
	explicit AudioManager(const Engine& engine);
	std::shared_ptr<Sound> create_sound(std::string, bool, float) const;
	void update();
private:
	const Engine& _engine;

	SDL_AudioDeviceID _audio_device;
	SDL_AudioSpec _audio_spec_from_file{};
	mutable std::vector<std::weak_ptr<Sound>> _buffers;

	static void audio_callback(void*, uint8_t*, int);
};