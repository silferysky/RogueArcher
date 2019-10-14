#pragma once
#include "BaseComponent.h"
#include "AABB.h"
#include "OBB.h"

class BoxCollider2DComponent
	: public BaseComponent
{
	AABB m_aabb;
	OBB m_obb;
public:
	AABB& AABB();
	OBB& OBB();

	std::string Serialize();
	void Deserialize(std::string toDeserialize);
};