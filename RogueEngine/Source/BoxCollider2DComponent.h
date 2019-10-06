#pragma once
#include "AABB.h"
#include "OBB.h"
#include <sstream>
#include "BaseComponent.h"

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