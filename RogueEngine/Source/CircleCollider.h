#pragma once
#include "BaseCollider.h"
#include "PhysicsDataStructures.hpp"

namespace Rogue
{
	class CircleCollider : public BaseCollider
	{
		CircleShape m_shape;
	public:
		CircleCollider() :
			m_shape{ 1.0f }
		{}

		float getRadius() const
		{
			return m_shape.m_radius;
		}

		void setRadius(float radius)
		{
			m_shape.m_radius = radius;
		}
	};
}