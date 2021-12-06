#pragma once
#include <vector>

class EventManager {
public:
	struct QuitEvent {};
	struct KeyDownEvent {};
	struct KeyUpEvent {};
	struct KeyLeftEvent {};
	struct KeyRightEvent {};
	struct KeyAEvent {};
	struct KeyDEvent {};

	struct Delegate
	{
		virtual void handle_event(QuitEvent) = 0;
		virtual void handle_event(KeyDownEvent) = 0;
		virtual void handle_event(KeyUpEvent) = 0;
		virtual void handle_event(KeyLeftEvent) = 0;
		virtual void handle_event(KeyRightEvent) = 0;
		virtual void handle_event(KeyAEvent) = 0;
		virtual void handle_event(KeyDEvent) = 0;
	};

	template<typename T>
	void invoke_event(T event) const
	{
		for (auto delegate : _delegates)
		{
			delegate->handle_event(event);
		}
	}

	void add_delegate(Delegate* delegate);
private:
	std::vector<Delegate*> _delegates;
};