#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <optional>

class Engine;

class Node : public std::enable_shared_from_this<Node>
{
public:
	explicit Node(const Engine&);

	virtual ~Node();
	
	void visit();
	void reverse_visit();

	void add_node(std::shared_ptr<Node>);
	void add_node(std::shared_ptr<Node>, int);
	
	void remove_node(std::shared_ptr<Node>);
	void remove_from_parent();

	Node* get_parent();

	const glm::vec2& get_position() const;
	void set_position(const glm::vec2&);

	const glm::vec2& get_scale() const;
	void set_scale(const glm::vec2&);

	glm::float32 get_rotation() const;
	void set_rotation(const float);

	const glm::vec2& get_anchor() const;
	void set_anchor(const glm::vec2&);

	const glm::vec2& get_size() const;
	void set_size(const glm::vec2&);

	glm::mat3 get_transform();
	std::vector<std::shared_ptr<Node>> get_children();

	int get_zOrder() const;
	void set_zOrder(int);

	std::optional<glm::mat3> _transform;
private:
	Node* _parent = nullptr;
	std::vector <std::shared_ptr<Node>> _nodes;

	glm::vec2 _pos = glm::vec2(0.0f);
	glm::vec2 _scale = glm::vec2(1.0f);
	glm::vec2 _anchor = glm::vec2(0.5f);
	glm::float32 _rot = 0.0f;

	int _zOrder = 0;

	bool _is_visible = true;
protected:
	virtual void visitSelf() {};
	
	const Engine& _engine;

	std::uint32_t _render_mask{ 0x0 };

	glm::vec2 _content_size = glm::vec2(0.0f);
};