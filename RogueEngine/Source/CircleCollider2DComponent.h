#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class CircleCollider2DComponent
		: public BaseComponent
	{
		float m_radius;
		float m_center;
	public:
		CircleCollider2DComponent() = default;
		~CircleCollider2DComponent() = default;

		float getRadius() const;
		float getCenter() const;
		void setRadius(float radius);
		void setCenter(float center);

		std::string Serialize();
		void Deserialize(std::string);
	};
}