#pragma once
#include "BaseCollider.h"
#include "PhysicsDataStructures.hpp"
#include "Vector2D.h"

namespace Rogue
{
	class BoundingCircle : public BaseCollider
	{
		float m_radius;
		Vec2 m_center;

	public:
		BoundingCircle() :
			BaseCollider(), m_radius{ 0.0f }, m_center{ Vec2{} }
		{}

		BoundingCircle(const BoundingCircle& rhs) :
			m_radius{ rhs.m_radius }, m_center{ rhs.m_center }
		{}
		
		BoundingCircle(BoundingCircle&& rhs) noexcept :
			BaseCollider(rhs), m_radius{ 0.0f }, m_center{ Vec2{} }
		{
			std::swap(m_radius, rhs.m_radius);
			std::swap(m_center, rhs.m_center);
		}

		BoundingCircle& operator=(const BoundingCircle& rhs)
		{
			if (this != &rhs)
			{
				m_radius = rhs.m_radius;
				m_center = rhs.m_center;
			}
			return *this;
		}

		BoundingCircle& operator=(BoundingCircle&& rhs) noexcept
		{
			if (this != &rhs)
			{
				std::swap(m_radius, rhs.m_radius);
				std::swap(m_center, rhs.m_center);
			}

			return *this;
		}

		float getRadius() const
		{
			return m_radius;
		}

		void setRadius(float radius)
		{
			m_radius = radius;
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