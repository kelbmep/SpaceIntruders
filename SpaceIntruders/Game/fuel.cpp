#include "fuel.hpp"
#include "userData.hpp"
#include <engine.hpp>
#include <physicalSprite.hpp>

Fuel::Fuel(const Engine& engine, UserData& userdata,float x, float y) : Node(engine), _userdata(userdata)
{
	b2BodyDef bd;
	bd.position.Set(x, y);

	_body = (&engine.get_world())->CreateBody(&bd);

	b2PolygonShape shape;
	shape.SetAsBox(1.0f, 1.0f);

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 0.0f;
	fd.isSensor = true;

	_body->CreateFixture(&fd);

	auto fuel_tank = std::make_shared<PhysicalSprite>(engine, _body, "fuel.png");
	fuel_tank->set_scale(glm::vec2(1.0f * 2.0f / fuel_tank->get_size()));

	this->add_node(fuel_tank);
}

void Fuel::visitSelf()
{
	for (b2ContactEdge* ce = _body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		if (ce->contact->IsTouching())
		{
			_userdata.set_fuel(1.0f);
			(&_engine.get_world())->DestroyBody(_body);
			this->remove_from_parent();
			break;
		}
	}
}