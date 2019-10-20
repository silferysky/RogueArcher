#pragma once
#include "BaseComponent.h"
#include "AABB.h"
#include "OBB.h"

namespace Rogue
{
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

		void operator=(const BoxCollider2DComponent& rhs);
	};
}
