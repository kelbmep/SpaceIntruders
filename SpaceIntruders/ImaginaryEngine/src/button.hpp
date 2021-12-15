#pragma once

#include <eventManager.hpp>
#include <node.hpp>

class Button : public EventManager::Delegate, public Node
{
public:
	Button();
	void handle_event(EventManager::QuitEvent);
private:

};