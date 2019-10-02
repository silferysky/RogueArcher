#pragma once
#include <cmath>
#include "json.hpp"

void init();
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