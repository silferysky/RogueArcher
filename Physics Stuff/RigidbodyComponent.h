#pragma once


class RigidbodyComponent // : public BaseComponent
{
	Vec2 m_pos;
	Vec2 m_vel;
	Vec2 m_force;
	float m_orientation;
	float m_angularVel;
	float m_torque;

	Shape m_shape;

	bool m_collidable;
	bool m_penetratable;

public:
	RigidbodyComponent()
	{}

	~RigidbodyComponent()
	{}

	void setForce(Vec2& force);

	friend class PhysicsSystem;
};

/*******************************************************************************

					   n1
					   ^
					   |
					   |
				p1 ^-------> p2
				   |       |
        n4 <------ |   C   | -------> n2
				   |       |
				p4 <-------v p3
				       |
					   |
					   v
					   n3


********************************************************************************/