#pragma once
#include <cmath>
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