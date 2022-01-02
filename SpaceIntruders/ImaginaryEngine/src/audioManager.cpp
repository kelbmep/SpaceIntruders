#include <audioManager.hpp>
#include <stdexcept>

AudioManager::AudioManager()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	_audio_spec_from_file.freq = 44100;
	_audio_spec_from_file.format = 32784;
	_audio_spec_from_file.channels = 2;
	_audio_spec_from_file.silence = 0;
	_audio_spec_from_file.samples = 4096;
	_audio_spec_from_file.padding = 0;
	_audio_spec_from_file.size = 0;
	_audio_spec_from_file.callback = this->audio_callback;
	_audio_spec_from_file.userdata = this;

	SDL_AudioSpec returned{};
	const char* device_name = nullptr;

	_audio_device = SDL_OpenAudioDevice(device_name, 0, &_audio_spec_from_file, &returned, 0);

	if (_audio_device == 0)
	{
		throw std::runtime_error("Can't open audio device.");
	}

	if (_audio_spec_from_file.format != returned.format
		|| _audio_spec_from_file.channels != returned.channels
		|| _audio_spec_from_file.freq != returned.freq)
	{
		throw std::runtime_error("Audio device doesn't support format.");
	}
}

void AudioManager::audio_callback(void* userdata, uint8_t* stream, int len)
{
	auto audioManager = static_cast<AudioManager*>(userdata);
	SDL_memset(stream, 0, len);
	for (auto& buffer : audioManager->_buffers)
	{
		auto sound = buffer.lock();
		if (!buffer.expired() && sound->is_playing())
		{
			auto amount = sound->_file_len - sound->_pos;
			if (amount > len)
			{
				amount = len;
			}
			SDL_MixAudioFormat(stream, sound->_data + sound->_pos, AUDIO_S16LSB, (Uint32)amount, (int)(sound->get_volume() * SDL_MIX_MAXVOLUME));

			sound->_pos += amount;

			if (sound->_pos >= sound->_file_len && sound->_is_loop)
			{
				sound->_pos = 0;
			}
			else if (sound->_pos >= sound->_file_len && !sound->_is_loop)
			{
				sound->stop();
			}
		}
	}
}

std::shared_ptr<Sound> AudioManager::create_sound(std::string file_name, bool is_loop, float volume) const
{
	auto sound = std::make_shared<Sound>(file_name, is_loop, volume);
	_buffers.push_back(sound);
	return sound;
}

void AudioManager::update()
{
	_buffers.erase(std::remove_if(_buffers.begin(), _buffers.end(), [](const std::weak_ptr<Sound>& s)
		{ return s.expired(); }), _buffers.end());

	if (_buffers.size() != 0)
	{
		auto is_play = false;

		for (auto& sound : _buffers)
		{
			if (sound.lock()->is_playing())
			{
				is_play = true;
				break;
			}
		}
		if (is_play)
		{
			SDL_PauseAudioDevice(_audio_device, SDL_FALSE);
		}
		else
		{
			SDL_PauseAudioDevice(_audio_device, SDL_TRUE);
		}
	}
	else
	{
		SDL_PauseAudioDevice(_audio_device, SDL_TRUE);
	}
}