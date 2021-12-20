#include <node.hpp>
#include <algorithm>
#include <glm/gtx/matrix_transform_2d.hpp>

void Node::add_node(std::shared_ptr<Node> n)
{
	if (n != nullptr)
	{
		n->_parent = this;
		//_nodes.push_back(std::move(n));
		_nodes.insert(std::upper_bound(_nodes.begin(), _nodes.end(), n, [](std::shared_ptr<Node> left, std::shared_ptr<Node> right) {
			return left->get_zOrder() < right->get_zOrder(); }), n);
	}
}

void Node::remove_node(std::shared_ptr<Node> n)
{
	auto it = std::find(_nodes.begin(), _nodes.end(), n);
	_nodes.erase(it);
}

void Node::remove_from_parent()
{
	_parent->remove_node(shared_from_this());
}

Node* Node::get_parent()
{
	return _parent;
}

void Node::visit()
{
	auto bound = std::upper_bound(_nodes.begin(), _nodes.end(), this, [](Node* left, std::shared_ptr<Node> right) {
		return left->get_zOrder() >= right->get_zOrder(); });
	for (auto it = _nodes.begin(); it != bound; ++it)
	{
		it->get()->visit();
	}
	this->visitSelf();
	for (auto it = bound ; it != _nodes.end(); ++it)
	{
		it->get()->visit();
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

		trans = glm::translate(trans, -glm::vec2(_anchor.x * _content_size.x, _anchor.y * _content_size.y));

		_transform = trans;
		return _parent ? (get_parent()->get_transform() * trans) : trans;
	}
}

const glm::vec2& Node::get_size() const
{
	return _content_size;
}

std::vector<std::shared_ptr<Node>> Node::get_children()
{
	return _nodes;
}

int Node::get_zOrder() const
{
	return _zOrder;
}

void Node::set_zOrder(int z)
{
	_zOrder = z;
}