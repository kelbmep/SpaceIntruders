#include <node.hpp>
#include <engine.hpp>
#include <algorithm>
#include <glm/gtx/matrix_transform_2d.hpp>

Node::Node(const Engine& engine)
	: _engine(engine)
	, _id(utils::gen_unique_object_ID())
{}

void Node::add_node(std::shared_ptr<Node> n)
{
	this->add_node(n, n->_zOrder);
}

void Node::add_node(std::shared_ptr<Node> n, int order)
{
	if (n != nullptr)
	{
		n->_parent = this;
		n->_zOrder = order;
		if (!_nodes.empty())
		{
			auto it = std::upper_bound(_nodes.begin(), _nodes.end(), order, [](auto order, auto n)
			{
				return n->_zOrder > order;
			});

			_nodes.insert(it, std::move(n));
		}
		else
		{
			_nodes.push_back(std::move(n));
		}
	}
}

void Node::remove_node(std::shared_ptr<Node> n)
{
	if (n != nullptr)
	{
		auto it = std::find(_nodes.begin(), _nodes.end(), n);

		if (it != _nodes.end())
		{
			n->_parent = nullptr;
			_nodes.erase(it);
		}
	}
}

void Node::remove_from_parent()
{
	if (this->_parent != nullptr)
	{
		_parent->remove_node(shared_from_this());
	}
}

Node* Node::get_parent()
{
	return _parent;
}

void Node::visit()
{
	if (_is_visible)
	{
		auto it = std::begin(_nodes);

		while (it != std::end(_nodes) && (*it)->get_zOrder() < 0)
		{
			it->get()->visit();
			it++;
		}

		visitSelf();

		if (_nodes.empty())
			return;

		while (it != std::end(_nodes))
		{
			auto help = _nodes.size();
			it->get()->visit();
			if (_nodes.size() < help)
				return; 
			it++;
		}
	}
}

void Node::reverse_visit()
{
	if (_is_visible)
	{
		auto it = _nodes.rbegin();

		while ((it != _nodes.rend()) && (*it)->_zOrder > 0)
		{
			it->get()->reverse_visit();
			it++;
		}

		visitSelf();

		while (it != _nodes.rend())
		{
			it->get()->reverse_visit();
			it++;
		}
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

void Node::set_size(const glm::vec2 &size)
{
	_content_size = size;
}

void Node::schedule_update()
{
	_engine.get_schedule_manager().schedule_update([this](fseconds delta)
		{
			this->update(delta);
		}, _id);
}

void Node::unschedule_update()
{
	_engine.get_schedule_manager().stop(_id);
}

uint32_t Node::get_render_mask() const
{
	return _render_mask;
}

void Node::set_render_mask(uint32_t render_mask)
{
	_render_mask = render_mask;
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

Node::~Node()
{
	//this->unschedule_update();
	for (auto &n : _nodes)
	{
		n->_parent = nullptr;
	}
}