#include "coin.hpp"
#include "userData.hpp"
#include <engine.hpp>
#include <physicalSprite.hpp>
#include <sound.hpp>

Coin::Coin(const Engine& engine, UserData& userdata, float x, float y) : Node(engine), _userdata(userdata)
{
	b2BodyDef bd;
	bd.position.Set(x, y);

	_body = (&engine.get_world())->CreateBody(&bd);

	b2CircleShape shape;
	shape.m_radius = _radius;

	b2FixtureDef fd;
	fd.shape = &shape;
	fd.density = 0.0f;
	fd.isSensor = true;

	_body->CreateFixture(&fd);

	auto coin = std::make_shared<PhysicalSprite>(engine, _body, "coin.png");
	coin->set_scale(glm::vec2(shape.m_radius * 2.0f / coin->get_size()));
	
	this->add_node(coin);
}

void Coin::visitSelf()
{
	for (b2ContactEdge* ce = _body->GetContactList(); ce != nullptr; ce = ce->next)
	{
		if (ce->contact->IsTouching())
		{
			_userdata.set_coins(_userdata.get_coins() + 10);
			_userdata.get_coin_sound()->stop();
			_userdata.get_coin_sound()->play();
			(&_engine.get_world())->DestroyBody(_body);
			this->remove_from_parent();
			break;
		}
	}
}