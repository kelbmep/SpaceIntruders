#include <actionManager.hpp>
#include <node.hpp>

Action::Action(const fseconds& duration)
    : _duration(duration)
{
}

void Action::start_with_target(Node* target)
{
    _target = target;
}

size_t Action::get_tag() const
{
    return _tag;
}

void Action::set_tag(size_t tag)
{
    _tag = tag;
}

void Action::step(fseconds dt)
{
    _elapsed += dt;
    this->update(glm::clamp(_elapsed.count() / _duration.count(), 0.0f, 1.0f));
}

Node* Action::get_target() const
{
    return _target;
}

bool Action::is_done()
{
    return _elapsed >= _duration;
}

MoveBy::MoveBy(glm::vec2 offset, fseconds duration)
    : _offset(offset)
    , Action(duration)
{
}

void MoveBy::update(float value)
{
    Action::update(value);
    if (_target)
    {
        auto delta = glm::mix(glm::vec2(0), _offset, value);
        _target->set_position(_start_position + delta);
    }
}

void MoveBy::start_with_target(Node* target)
{
    Action::start_with_target(target);
    _start_position = target->get_position();
}

void ActionManager::add_action(std::shared_ptr<Action> action) const
{
    _actions.push_back(std::move(action));
}

void ActionManager::remove_action(const std::shared_ptr<Action>& action) const
{
    auto it = std::find(_actions.begin(), _actions.end(), action);
    _actions.erase(it);
}

void ActionManager::remove_action_by_tag(size_t tag) const
{
    _actions.erase(std::remove_if(_actions.begin(), _actions.end(), [&](const std::shared_ptr<Action>& action)
        {
            return action->get_tag() == tag;
        }));
}

void ActionManager::remove_action_for_node(Node* node) const
{
    _actions.erase(std::remove_if(_actions.begin(), _actions.end(), [&](const std::shared_ptr<Action>& action)
        {
            return action->get_target() == node;
        }));
}

void ActionManager::update(fseconds dt) const
{
    for (auto& action : _actions)
    {
        action->step(dt);
        if (action->is_done())
        {
            auto it = std::find(_actions.begin(), _actions.end(), action);
            _actions.erase(it);
        }
    }
}