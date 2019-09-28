#pragma once
#include "./Physics/AABB.h"
#include "./Physics/OBB.h"

class BoxCollider2D
{
public:
	AABB m_aabb;
	OBB m_obb;
};

