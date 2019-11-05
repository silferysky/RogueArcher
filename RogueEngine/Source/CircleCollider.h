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
			BaseCollider(), m_shape{ 1.0f }, m_center{ Vec2{} }
		{}

		CircleCollider(const CircleCollider& rhs) :
			m_shape{ rhs.m_shape }, m_center{ rhs.m_center }
		{}
		
		CircleCollider(CircleCollider&& rhs) noexcept :
			BaseCollider(rhs), m_shape{ CircleShape() }, m_center{ Vec2{} }
		{
			std::swap(m_shape, rhs.m_shape);
			std::swap(m_center, rhs.m_center);
		}

		CircleCollider& operator=(const CircleCollider& rhs)
		{
			if (this != &rhs)
			{
				m_shape = rhs.m_shape;
				m_center = rhs.m_center;
			}
			return *this;
		}

		CircleCollider& operator=(CircleCollider&& rhs) noexcept
		{
			if (this != &rhs)
			{
				std::swap(m_shape, rhs.m_shape);
				std::swap(m_center, rhs.m_center);
			}

			return *this;
		}


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