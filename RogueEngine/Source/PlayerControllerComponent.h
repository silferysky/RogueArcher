#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	class PlayerControllerComponent
		: public BaseComponent
	{
		Vec2 m_moveSpeed;
		float m_slowTime;
		bool m_isActive;
	public:
		PlayerControllerComponent() = default;
		~PlayerControllerComponent() = default;
		void DisplayOnInspector();
		void SetSlowTime(const float& slowTime);
		float GetSlowTime() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};

}