#pragma once
#include "AABB.h"
#include "OBB.h"

class BoxCollider2DComponent
{
public:
	AABB m_aabb;
	OBB m_obb;
};

