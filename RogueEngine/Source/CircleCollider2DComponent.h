/*Start Header*************************************************************************/
/*!
\file           CircleCollider2DComponent.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CircleCollider2DComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"
#include "BaseCollider.h"
#include "BoundingCircle.h"

namespace Rogue
{
	class CircleCollider2DComponent
		: public BaseComponent
	{
		CollisionMode m_collisionMode;
		bool m_isCollided;
	public:
		CircleCollider2DComponent() = default;
		~CircleCollider2DComponent() = default;

		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
		CircleCollider2DComponent(CircleCollider2DComponent&& rhs) noexcept;

		CircleCollider2DComponent& operator=(const CircleCollider2DComponent&);
		CircleCollider2DComponent& operator=(CircleCollider2DComponent&& rhs) noexcept;

		BoundingCircle m_collider;

		CollisionMode GetCollisionMode() const;
		void SetCollisionMode(CollisionMode mode);
		bool GetIsCollided() const;
		void SetIsCollided(bool isCollided);

		void DisplayOnInspector();
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}