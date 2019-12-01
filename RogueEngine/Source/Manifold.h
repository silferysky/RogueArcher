/*Start Header*************************************************************************/
/*!
\file           Manifold.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for Manifold

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Vector2D.h"
#include <array>
#include "Types.h"

namespace Rogue
{
	struct Manifold
	{
		Vec2 m_point;
		Vec2 m_normal;
		Vec2 m_normal2; // Used for OBB
		float m_penetration;
		float m_restitution;

		std::array<Vec2, 2> m_contactPoints;

		Entity m_entityA;
		Entity m_entityB;

		Manifold(Entity a, Entity b);
		~Manifold() = default;

		void ResolveImpulse();
		void ResolveFriction();
		void PositionalCorrection();
	};
}
