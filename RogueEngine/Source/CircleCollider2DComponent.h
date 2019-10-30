#pragma once
#include "BaseComponent.h"
#include "BaseCollider.h"
#include "CircleCollider.h"

namespace Rogue
{
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