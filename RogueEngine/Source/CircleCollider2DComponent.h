#pragma once
#include "BaseComponent.h"
#include "BaseCollider.h"

namespace Rogue
{
	class CircleCollider : public BaseCollider
	{
		float m_radius;
	public:
		CircleCollider() :
			m_radius{ 1.0f }
		{}

		float getRadius() const
		{
			return m_radius;
		}

		void setRadius(float radius)
		{
			m_radius = radius;
		}
	};

	class CircleCollider2DComponent
		: public BaseComponent
	{
	public:
		CircleCollider2DComponent() = default;
		~CircleCollider2DComponent() = default;

		CircleCollider m_collider;

		std::string Serialize();
		void Deserialize(std::string);
	};
}