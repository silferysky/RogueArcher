#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	enum class MoveState
	{
		e_left = 0,
		e_stop,
		e_right
	};

	class PlayerControllerComponent
		: public BaseComponent
	{
		float m_moveSpeed;
		float m_slowTime;
		bool m_isActive;
		MoveState m_moveState;

	public:
		PlayerControllerComponent();
		~PlayerControllerComponent() = default;

		void DisplayOnInspector();

		void SetSlowTime(float slowTime);
		void SetMoveSpeed(float speed);
		void SetMoveState(MoveState state);

		float GetSlowTime() const;
		float GetMoveSpeed() const;
		MoveState GetMoveState() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};

}