#pragma once
#include "BaseComponent.h"
#include "AABB.h"
#include "OBB.h"
#include "PhysicsDataStructures.hpp"

namespace Rogue
{
	class BoxCollider2DComponent : public BaseComponent
	{
		bool m_rotatable;
	public:
		AABB m_aabb;
		OBB m_obb;
		//std::unique_ptr<Shape> m_shape;

		BoxCollider2DComponent();
		BoxCollider2DComponent(const BoxCollider2DComponent& rhs); // Copy constructor
		BoxCollider2DComponent(BoxCollider2DComponent&& rhs); // Move constructor
		BoxCollider2DComponent& operator=(const BoxCollider2DComponent& rhs); // Copy assignment
		BoxCollider2DComponent& operator=(BoxCollider2DComponent&& rhs); // Move assignment

		bool Rotatable() const;
		void setRotatable(bool set);

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}