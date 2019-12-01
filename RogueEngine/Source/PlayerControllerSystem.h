/* Start Header ************************************************************************/
/*!
\file           PlayerControllerSystem.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PlayerControllerSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "Vector2D.h"
#include "SpecialEntity.h"
#include <memory>

#define FORCE_FACTOR 80000
#define POSITION_RELATIVITY 10

namespace Rogue
{
	class PlayerControllerSystem
		: public System, public EventListener
	{
	public:

		PlayerControllerSystem();
		~PlayerControllerSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event * ev) override;
		void Shutdown() override;

		std::vector<TimedEntity> GetTimedEntities() const;
		void AddToTimedEntities(TimedEntity newEnt);
		void AddToTimedEntities(Entity entity, float duration = 1.0f);
		void ClearTimedEntities();
		void ClearTeleportEntities();

		void CreateTeleportEvent(Vec2 newPosition);

		void setInLight(float duration = 0.16f);

	private:
		void CreateBallAttack();
		std::vector<TimedEntity> m_timedEntities;
		std::vector<TimedEntity> m_teleports;
		float m_isInLight;
		const float m_maxJumpTimer;
	};
}