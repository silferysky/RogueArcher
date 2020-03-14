/* Start Header ************************************************************************/
/*!
\file           PhysicsSystem.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PhysicsSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseSystem.h"
#include "EventListener.h"
#include "Vector2D.h"
#include "ForceManager.h"

namespace Rogue
{
	class Timer;
	class RigidbodyComponent;
	class TransformComponent;
	class PlayerControllerComponent;

	class PhysicsSystem : public System, public EventListener
	{
		Vec2 m_gravity;

		void Integrate(RigidbodyComponent& rigidbody, TransformComponent& transform) const;

		// For debugging.
		bool checkAABB;
		bool checkOBB;
		bool allowGravity;

		// Temporary place to store movement script
		Vec2 PlayerMoveByForce(PlayerControllerComponent& playerCtrl, RigidbodyComponent& rigidbody, Vec2 vecDir);
		void PlayerMoveByVelocity(PlayerControllerComponent& playerCtrl, RigidbodyComponent& rigidbody, const Vec2& vecDir);
	public:
		PhysicsSystem(Vec2 gravity = { 0.0f, -9.81f });
		~PhysicsSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;

		bool getToggleGravity();
		void setToggleGravity(bool gravity);
		void setGravity(const Vec2& gravity);
		const Vec2& getGravity() const;

		int m_playerMove;
	};
}