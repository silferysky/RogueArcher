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
#include "AABB.h"
#include "TransformComponent.h"
#include "REMath.h"
#include <memory>

#define FORCE_FACTOR 80000
#define POSITION_RELATIVITY 10
#define TELEPORT_DELAY 0.2f

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
		void Receive(Event& ev) override;
		void Shutdown() override;

		void CreateTeleportEvent(Vec2 newPosition);
		void Hitchhike(Entity ent);
	private:
		void Teleport();
		Vec2 GetTeleportRaycast();
		Entity HitchhikeRaycast();
		void ToggleMode();

		void FreezeControlComponentUpdates();
		void UnfreezeControlComponentUpdates();

		//Macros to help Player do call events faster
		void ResetPlayerParent();
		void SetPlayerParent(Entity newParent);
		void MovingPlayer();

		void DebugDrawArrow(const LineSegment& line) const;

		bool m_ignoreFrameEvent;
	};
}