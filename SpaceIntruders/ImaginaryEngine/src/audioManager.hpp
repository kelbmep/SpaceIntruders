#pragma once

#include <sound.hpp>
#include <vector>

class AudioManager 
{
public:
	AudioManager();
	std::shared_ptr<Sound> create_sound(std::string, bool, float) const;
	void update();
private:
	SDL_AudioDeviceID _audio_device;
	SDL_AudioSpec _audio_spec_from_file{};
	mutable std::vector<std::weak_ptr<Sound>> _buffers;

	static void audio_callback(void*, uint8_t*, int);
};