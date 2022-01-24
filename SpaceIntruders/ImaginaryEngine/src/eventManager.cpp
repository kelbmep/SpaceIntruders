#include <eventManager.hpp>

void EventManager::add_delegate(Delegate* delegate) const
{
	_delegates.push_back(delegate);
}