/*Start Header*************************************************************************/
/*!
\file           BoxCollider2DComponent.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BoxCollider2DComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"
#include "AABB.h"
#include "PhysicsDataStructures.hpp"

namespace Rogue
{
	class BoxCollider2DComponent : public BaseComponent
	{
		CollisionMode m_collisionMode;
		bool m_isCollided;
	public:
		AABB m_aabb;

		BoxCollider2DComponent();
		BoxCollider2DComponent(const BoxCollider2DComponent& rhs); // Copy constructor
		BoxCollider2DComponent(BoxCollider2DComponent&& rhs) noexcept; // Move constructor
		BoxCollider2DComponent& operator=(const BoxCollider2DComponent& rhs); // Copy assignment
		BoxCollider2DComponent& operator=(BoxCollider2DComponent&& rhs) noexcept; // Move assignment

		CollisionMode GetCollisionMode() const;
		void SetCollisionMode(CollisionMode mode);
		bool GetIsCollided() const;
		void SetIsCollided(bool isCollided);

		void DisplayOnInspector();
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}