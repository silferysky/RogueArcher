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

		void SetSlowTime(float slowTime);
		void SetMoveSpeed(const Vec2& speed);
		void SetMoveSpeedX(float x);
		void SetMoveSpeedY(float y);

		float GetSlowTime() const;
		Vec2 GetMoveSpeed() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};

}