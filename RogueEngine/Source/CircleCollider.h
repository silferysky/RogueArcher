#pragma once
#include "BaseCollider.h"
#include "PhysicsDataStructures.hpp"
#include "Vector2D.h"

namespace Rogue
{
	class CircleCollider : public BaseCollider
	{
		CircleShape m_shape;
		Vec2 m_center;

	public:
		CircleCollider() :
			m_shape{ 1.0f }, m_center{ Vec2{} }
		{}

		float getRadius() const
		{
			return m_shape.m_radius;
		}

		void setRadius(float radius)
		{
			m_shape.m_radius = radius;
		}

		const Vec2& GetCenter() const
		{
			return m_center;
		}
	
		void SetCenter(const Vec2& center)
		{
			m_center = center;
		}
	};
}