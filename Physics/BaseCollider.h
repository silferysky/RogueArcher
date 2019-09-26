#pragma once

class BaseCollider
{
public:
	BaseCollider()
	{}
	~BaseCollider()
	{}
};

enum COLLIDERS
{
	COLLIDER_BC = 0,
	COLLIDER_AABB,
	COLLIDER_OBB,

	MAX_COLLIDERS
};