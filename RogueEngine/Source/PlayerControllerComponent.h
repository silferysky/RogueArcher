/* Start Header ************************************************************************/
/*!
\file           PlayerControllerComponent.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PlayerControllerComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		bool m_grounded;

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