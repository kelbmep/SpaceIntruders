#include "ground.hpp"
#include <sprite.hpp>
#include <engine.hpp>

Ground::Ground(const Engine& engine, float ground_level) : _ground_level(ground_level)
{
	auto scaler = 0.05f;
	
	auto dirt = std::make_shared<Sprite>(engine, "dirt.jpg", glm::vec2(80.0f, 5.0f));
	auto& sprite_size = dirt->get_size();

	float window_center = 80.0f / 2.0f * scaler * sprite_size.x;

	dirt->set_scale(glm::vec2(80.0f, 5.0f) * scaler);
	dirt->set_position(glm::vec2(window_center, ground_level + sprite_size.y / 8.0f));
	engine.get_scene()->add_node(dirt);

	b2BodyDef bd;
	_ground = (&engine.get_world())->CreateBody(&bd);

	b2EdgeShape shape;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 0.0f;
	fd.friction = 0.6f;

	shape.SetTwoSided(b2Vec2(0.0f, ground_level), b2Vec2(0.0f, 0.0f)); // invisible wall
	_ground->CreateFixture(&fd);

	shape.SetTwoSided(b2Vec2(0.0f, ground_level), b2Vec2(window_center * 2.0f, ground_level)); // ground level

	_ground->CreateFixture(&fd);

	float x = 365.0f;
	float dx = sprite_size.x * scaler;
	float y1 = 0.0f;

	float hs[5] = { 0.2f, 0.5f, 0.3f, 0.1f, 0.0f };

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 5; i++)
		{
			float y2 = hs[i];
			auto dirt = std::make_shared<Sprite>(engine, "dirt.jpg", y1, y2);
			dirt->set_scale(glm::vec2(scaler));
			dirt->set_position(glm::vec2(x + dx / 2, ground_level + sprite_size.y * scaler / 2));
			engine.get_scene()->add_node(dirt);

			shape.SetTwoSided(b2Vec2(x, ground_level - y1 * sprite_size.y * scaler), b2Vec2(x + dx, ground_level - y2 * sprite_size.y * scaler));
			_ground->CreateFixture(&fd);
			x += dx;
			y1 = y2;
		}
		x += 30.0f;
	}
}

float Ground::calculate_height(float x) const //TODO
{
	return 0.0f;
	//x / dx * y2 - y1 + min(y1, y2);
}