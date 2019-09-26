#pragma once
#include "ComponentList.h"

struct Entity;

struct BaseComponent
{
	Entity* _owner;
	ComponentType _cmpType;
	size_t _id;
	bool _isActive;

public:
	BaseComponent(ComponentType id = CMP_BASE, Entity* ent = nullptr)
		: _cmpType{ id }, _owner{ ent } {_isActive = true; }

	virtual ~BaseComponent() = default; //Delete is handled not here

	/*void owner(Entity* ent) { _owner = ent; }
	Entity* owner() const { return _owner; }

	void type(const ComponentType& id) { _cmpType = id; }
	ComponentType type() const { return _cmpType; }*/
};

/*class RigidBody : public BaseComponent
{
	//Can add in Position/Center of gravity/rotation in future for rigidbody not following Transform
	Vec2 _mass;
	Vec2 _velocity;
	Vec2 _acceleration;

public:
	void mass(Vec2 m) { _mass = m; }
	Vec2 mass() { return _mass; }
	void velocity(Vec2 v) { _velocity = v; }
	Vec2 velocity() { return _velocity; }
	void acceleration(Vec2 a) { _acceleration = a; }
	Vec2 acceleration() { return _acceleration; }
};*/