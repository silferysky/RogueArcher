#pragma once
#include "AABB.h"
#include "OBB.h"

class BoxCollider2DComponent
{
	AABB m_aabb;
	OBB m_obb;
public:
	AABB& AABB();
	OBB& OBB();
};