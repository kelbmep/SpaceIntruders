#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <vector>
#include <optional>

class Node : public std::enable_shared_from_this<Node>
{
public:
	virtual ~Node() = default;
	void visit();
	void add_node(std::shared_ptr<Node>);
	void remove_node(std::shared_ptr<Node>);
	void remove_from_parent();

	Node* get_parent();

	const glm::vec2& get_position() const;
	void set_position(const glm::vec2& pos);

	const glm::vec2& get_scale() const;
	void set_scale(const glm::vec2& scale);

	glm::float32 get_rotation() const;
	void set_rotation(const float rot);

	const glm::vec2& get_anchor() const;
	void set_anchor(const glm::vec2& anchor);

	const glm::vec2& get_size() const;

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
protected:
	virtual void visitSelf() {};

	glm::vec2 _content_size = glm::vec2(0.0f);
};