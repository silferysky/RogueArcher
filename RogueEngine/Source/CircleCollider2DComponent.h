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

		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
		CircleCollider2DComponent(CircleCollider2DComponent&& rhs) noexcept;

		CircleCollider2DComponent& operator=(const CircleCollider2DComponent&);
		CircleCollider2DComponent& operator=(CircleCollider2DComponent&& rhs) noexcept;

		CircleCollider m_collider;

		void DisplayOnInspector();
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}