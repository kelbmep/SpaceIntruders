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
	void addNode(std::shared_ptr<Node>);
	void removeNode(std::shared_ptr<Node>);
	void removeFromParent();

	std::shared_ptr<Node> get_parent();

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
protected:
	virtual void visitSelf() {};

	std::shared_ptr<Node> _parent;
	std::vector <std::shared_ptr<Node>> _nodes;

	glm::vec2 _pos = glm::vec2(0.0f);
	glm::vec2 _scale = glm::vec2(1.0f);
	glm::vec2 _anchor = glm::vec2(0.5f);
	glm::float32 _rot = 0.0f;

	glm::vec2 _contentSize = glm::vec2(0.0f);
	std::optional<glm::mat3> _transform;
};