#include "car.hpp"
#include "userdata.hpp"
#include <../src/engine.hpp>
#include <../src/physicalSprite.hpp>

Car::Car(const Engine& engine, UserData& userdata, float x, float y) : Node(engine), _userdata(userdata)
{
	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	b2PolygonShape chassis;
	b2Vec2 vertices[8]{};
	vertices[0].Set((0.0f - 682.0f) / _scale, (98.0f - 152.5f) / _scale);
	vertices[1].Set((121.0f - 682.0f) / _scale, (0.0f - 152.5f) / _scale);
	vertices[2].Set((1364.0f - 682.0f) / _scale, (0.0f - 152.5f) / _scale);
	vertices[3].Set((1364.0f - 682.0f) / _scale, (136.0f - 152.5f) / _scale);
	vertices[4].Set((739.0f - 682.0f) / _scale, (305.0f - 152.5f) / _scale);
	vertices[5].Set((525.0f - 682.0f) / _scale, (305.0f - 152.5f) / _scale);
	vertices[6].Set((0.0f - 682.0f) / _scale, (216.0f - 152.5f) / _scale);
	chassis.Set(vertices, 7);

	b2CircleShape circle1;
	circle1.m_radius = 105.0f / _scale;

	b2CircleShape circle2;
	circle2.m_radius = 100.0f / _scale;

	b2BodyDef bd;
	bd.type = b2_dynamicBody;
	bd.position.Set(x, y - 10.0f / _scale);
	m_car = (&engine.get_world())->CreateBody(&bd);
	m_car->CreateFixture(&chassis, 1.0f);

	_car = std::make_shared<PhysicalSprite>(engine, m_car, "lamborghini.png");
	_car->set_scale(glm::vec2(1.0f) / _scale);
	
	b2FixtureDef fd1;
	fd1.shape = &circle1;
	fd1.density = 1.0f;
	fd1.friction = 0.9f;

	b2FixtureDef fd2;
	fd2.shape = &circle2;
	fd2.density = 1.0f;
	fd2.friction = 0.9f;

	bd.position.Set((232.0f - 682.0f) / _scale + x, y - ((72.0f - 152.5f) / _scale));//310 - 232
	m_wheel1 = (&engine.get_world())->CreateBody(&bd);
	m_wheel1->CreateFixture(&fd1);

	_back_wheel = std::make_shared<PhysicalSprite>(engine, m_wheel1, "back_wheel.png");
	_back_wheel->set_scale(glm::vec2(1.0f) / _scale);

	bd.position.Set((1054.0f - 682.0f) / _scale + x, y - ((66.0f - 152.5f) / _scale));//1131 - 1054
	m_wheel2 = (&engine.get_world())->CreateBody(&bd);
	m_wheel2->CreateFixture(&fd2);

	_front_wheel = std::make_shared<PhysicalSprite>(engine, m_wheel2, "front_wheel.png");
	_front_wheel->set_scale(glm::vec2(1.0f) / _scale);

	this->add_node(_car);
	this->add_node(_back_wheel);
	this->add_node(_front_wheel);

	b2WheelJointDef jd;
	b2Vec2 axis(0.0f, 1.0f);

	float mass1 = m_wheel1->GetMass();
	float mass2 = m_wheel2->GetMass();

	float hertz = 4.0f;
	float dampingRatio = 0.7f;
	float omega = 2.0f * b2_pi * hertz;

	jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 20.0f;
	jd.enableMotor = true;
	jd.stiffness = mass1 * omega * omega;
	jd.damping = 2.0f * mass1 * dampingRatio * omega;
	jd.lowerTranslation = -0.25f;
	jd.upperTranslation = 0.25f;
	jd.enableLimit = true;
	_m_spring1 = (b2WheelJoint*)(&engine.get_world())->CreateJoint(&jd);

	jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
	jd.motorSpeed = 0.0f;
	jd.maxMotorTorque = 10.0f;
	jd.enableMotor = false;
	jd.stiffness = mass2 * omega * omega;
	jd.damping = 2.0f * mass2 * dampingRatio * omega;
	jd.lowerTranslation = -0.25f;
	jd.upperTranslation = 0.25f;
	jd.enableLimit = true;
	_m_spring2 = (b2WheelJoint*)(&engine.get_world())->CreateJoint(&jd);
}

void Car::handle_event(KeyEvent e)
{
	if (e.key == KeyCode::A)
	{
		_isLeft = (e.type == KeyType::KeyDown);
	}

	if (e.key == KeyCode::D)
	{
		_isRight = (e.type == KeyType::KeyDown);
	}
}

std::shared_ptr<PhysicalSprite> Car::get_car_body() const
{
	return _car;
}

void Car::visitSelf()
{
	if (_isRight && _userdata.get_fuel() > 0)
	{
		_speed = 25.0f;
	}
	else if (_isLeft && _userdata.get_fuel() > 0)
	{
		_speed = -10.0f;
	}
	else
	{
		_speed = 0.0f;
	}
	_m_spring1->SetMotorSpeed(_speed);
}