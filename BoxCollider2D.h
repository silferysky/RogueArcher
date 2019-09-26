#pragma once
#include "./Physics/AABB.h"
#include "./Physics/OBB.h"

class BoxCollider2D
{
	AABB m_aabb;
	OBB m_obb;
};

