#include <playlist.hpp>
#include <sound.hpp>
#include <engine.hpp>
#include <audioManager.hpp>

Playlist::Playlist(const Engine& engine) : _engine{ engine }
{
}

void Playlist::add_track(std::string file_path)
{
	auto song = _engine.get_audio_manager().create_sound(file_path, false, 0.8f);
	if (_songs.size() == 0)
		_current_song = song;
	_songs.push_back(song);
}

void Playlist::play()
{
	_current_song.get()->play();
}

void Playlist::pause()
{
	_current_song.get()->pause();
}

void Playlist::play_previous()
{
	_current_song.get()->stop();
	auto it = std::find(_songs.begin(), _songs.end(), _current_song);
	if (it == _songs.begin())
	{
		_current_song = _songs.back();
	}
	else
	{
		_current_song = *(it - 1);
	}
	_current_song.get()->play();
}

void Playlist::play_next()
{
	_current_song.get()->stop();
	auto it = std::find(_songs.begin(), _songs.end(), _current_song);
	if (*it == _songs.back())
	{
		_current_song = *_songs.begin();
	}
	else
	{
		_current_song = *(it + 1);
	}
	_current_song.get()->play();
}

void Playlist::update()
{
	if (_previous)
	{
		play_previous();
		_previous = false;
	}
	else if (_next)
	{
		play_next();
		_next = false;
	}
	if (_current_song.get()->_state == Sound::State::Stop)
	{
		play_next();
	}
}

void Playlist::handle_event(KeyEvent e)
{
	if (e.key == KeyCode::NUM_9 && e.type == KeyType::KeyUp)
	{
		_previous = true;
	}
	
	if (e.key == KeyCode::NUM_0 && e.type == KeyType::KeyUp)
	{
		_next = true;
	}
}

void Playlist::set_volume(int volume)
{
	for (auto& song : _songs)
	{
		song.get()->set_volume(volume / 100.0f);
	}
}