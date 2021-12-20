#pragma once

#include <eventManager.hpp>
#include <node.hpp>
#include <string>

class Engine;
class Sprite;

class Button : public EventManager::Delegate, public Node
{
public:
	explicit Button(const Engine&, std::string);
	void handle_event(EventManager::MouseEvent) override;
	bool get_state() const;
protected:
	void visitSelf() override;

private:
	bool _state = false;
	std::shared_ptr<Sprite> _button;
};