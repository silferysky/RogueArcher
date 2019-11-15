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
		CollisionMode m_collisionMode;

	public:
		AABB m_aabb;
		OBB m_obb;

		BoxCollider2DComponent();
		BoxCollider2DComponent(const BoxCollider2DComponent& rhs); // Copy constructor
		BoxCollider2DComponent(BoxCollider2DComponent&& rhs) noexcept; // Move constructor
		BoxCollider2DComponent& operator=(const BoxCollider2DComponent& rhs); // Copy assignment
		BoxCollider2DComponent& operator=(BoxCollider2DComponent&& rhs) noexcept; // Move assignment

		bool Rotatable() const;
		void setRotatable(bool set);
		CollisionMode GetCollisionMode() const;
		void SetCollisionMode(CollisionMode mode);

		void DisplayOnInspector();
		std::string_view Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}