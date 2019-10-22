#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class CircleCollider2DComponent
		: public BaseComponent
	{
		float m_radius;
	public:
		CircleCollider2DComponent() = default;
		~CircleCollider2DComponent() = default;

		float getRadius() const;
		void setRadius(float radius);

		std::string Serialize();
		void Deserialize(std::string);
	};
}