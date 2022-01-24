#pragma once

#include <scheduleManager.hpp>
#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Node;
class Action 
{
public:
	explicit Action(const fseconds&);
	virtual void start_with_target(Node*);

	size_t get_tag() const;
	void set_tag(size_t);

	void step(fseconds);
	virtual void update(float) {};

	Node* get_target() const;

	bool is_done();
protected:
	fseconds _duration;
	fseconds _elapsed = 0s;
	size_t _tag{ 0 };

	Node* _target;
};

class MoveBy : public Action
{
public:
	MoveBy(glm::vec2, fseconds);

	void start_with_target(Node*) override;

	void update(float) override;
private:
	glm::vec2 _offset;
	glm::vec2 _start_position;
};

class ActionManager
{
public:
	void add_action(std::shared_ptr<Action>) const;
	void remove_action(const std::shared_ptr<Action>&) const;
	void remove_action_by_tag(size_t) const;
	void remove_action_for_node(Node*) const;

	void update(fseconds) const;
private:
	mutable std::vector<std::shared_ptr<Action>> _actions;
};