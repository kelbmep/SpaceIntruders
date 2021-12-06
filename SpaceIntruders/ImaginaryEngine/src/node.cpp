#include <node.hpp>
#include "glm/gtx/matrix_transform_2d.hpp"

void Node::addNode(std::shared_ptr<Node> n)
{
	if (n != nullptr)
	{
		n->_parent = shared_from_this();
		_nodes.push_back(std::move(n));
	}
}

void Node::removeNode(std::shared_ptr<Node> n)
{
	auto it = std::find(_nodes.begin(), _nodes.end(), n);
	_nodes.erase(it);
}

void Node::removeFromParent()
{
	_parent->removeNode(shared_from_this());
}

std::shared_ptr<Node> Node::get_parent()
{
	return _parent;
}

void Node::visit()
{
	this->visitSelf();
	for (auto n:_nodes)
	{
		n->visit();
	}
}

const glm::vec2& Node::get_position() const
{
	return _pos;
}

void Node::set_position(const glm::vec2& pos)
{
	_transform = std::nullopt;
	_pos = pos;
}

const glm::vec2& Node::get_scale() const
{
	return _scale;
}

void Node::set_scale(const glm::vec2& scale)
{
	_transform = std::nullopt;
	_scale = scale;
}

glm::float32 Node::get_rotation() const
{
	return _rot;
}

void Node::set_rotation(float rot)
{
	_transform = std::nullopt;
	_rot = rot;
}

const glm::vec2& Node::get_anchor() const
{
	return _anchor;
}

void Node::set_anchor(const glm::vec2& anchor)
{
	_transform = std::nullopt;
	_anchor = anchor;
}

glm::mat3 Node::get_transform()
{
	if (_transform)
	{
		return _parent ? (get_parent()->get_transform() * *_transform) : *_transform;
	}
	else
	{
		glm::mat3 trans(1.0f);

		trans = glm::translate(trans, _pos);
		trans = glm::scale(trans, _scale);
		trans = glm::rotate(trans, glm::radians(_rot));

		trans = glm::translate(trans, -glm::vec2(_anchor.x * _contentSize.x, _anchor.y * _contentSize.y));

		_transform = trans;
		return _parent ? (get_parent()->get_transform() * trans) : trans;
	}
}

const glm::vec2& Node::get_size() const
{
	return _contentSize;
}

std::vector<std::shared_ptr<Node>> Node::get_children()
{
	return _nodes;
}