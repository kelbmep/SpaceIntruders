#pragma once

#include <eventManager.hpp>

void EventManager::add_delegate(Delegate* delegate)
{
	_delegates.push_back(delegate);
}