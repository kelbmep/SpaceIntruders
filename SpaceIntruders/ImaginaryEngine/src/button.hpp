#pragma once

#include <eventManager.hpp>
#include <node.hpp>

class Engine;
class Sprite;

class Button : public EventManager::Delegate, public Node
{
public:
	explicit Button(const Engine&, std::string, float, float, bool = false, float = 1.0f);
	void handle_event(MouseEvent) override;
	bool get_state() const;
protected:
	void visitSelf() override;
private:
	bool _state = false;
	float _scale = 1.0f;
	std::shared_ptr<Sprite> _button;
};