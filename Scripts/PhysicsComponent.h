#pragma once
#include <cmath>
#include "BaseComponent.h"

struct PhysicsComponent : public BaseComponent
{
	struct Gravity
	{
		float force;
	};

	struct RigidBody
	{
		float velocity;
		float acceleration;
	};

	struct Transform
	{
		float position;
		float rotation;
		float scale;
	};
};