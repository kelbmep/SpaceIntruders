#pragma once

#include <vector>

class EventManager {
public:

	enum class KeyCode
	{
		Up,
		Down,
		Left,
		Right,
		A,
		D,
		Space,
		Escape,
		Unknown
	};

	enum class KeyType
	{
		KeyDown,
		KeyUp
	};

	struct KeyEvent 
	{
		KeyCode key;
		KeyType type;
	};

	struct MouseEvent
	{
		int x, y;
		KeyType type;
	};

	struct QuitEvent {};
	
	struct Delegate
	{
		virtual void handle_event(KeyEvent) {};
		virtual void handle_event(QuitEvent) {};
		virtual void handle_event(MouseEvent) {};
	};

	template<typename T>
	void invoke_event(T event) const
	{
		for (auto delegate : _delegates)
		{
			delegate->handle_event(event);
		}
	}

	void add_delegate(Delegate* delegate) const;
private:
	mutable std::vector<Delegate*> _delegates;
};