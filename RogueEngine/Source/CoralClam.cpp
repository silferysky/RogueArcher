#include "Precompiled.h"
#include "CoralClam.h"
#include "PlayerStatusManager.h"
//#include "Main.h"	//For g_deltaTime and coordinator

namespace Rogue
{
	CoralClam::CoralClam(Entity entity, LogicComponent& logicComponent, StatsComponent& statsComponent)
		: ScriptComponent(entity, logicComponent, statsComponent), m_timer{}, m_open{ 0 }
	{
	}

	void CoralClam::AIActiveStateUpdate()
	{
		if (!g_engine.m_coordinator.GameIsActive())
			return;

		m_logicComponent->SetActiveStateBit(static_cast<size_t>(AIState::AIState_Idle));
	}

	void CoralClam::AIIdleUpdate()
	{
		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(m_entity);
		auto& animation = g_engine.m_coordinator.GetComponent<AnimationComponent>(m_entity);
		auto& boxCollider = g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(m_entity);

		m_timer += g_deltaTime * g_engine.GetTimeScale();

		std::cout << animation.getCurrentFrame() << std::endl;

		if (m_open == 0) // if open
		{
			if (m_timer < 0.40f) // animation has ended
				boxCollider.SetCollisionMode(CollisionMode::e_trigger);
			else
			{
				m_open = 1;
				m_timer = 0.0f; // reset the timer 
				//sprite.setTexturePath("Resources/Assets/CoralClamClose.png");
				std::cout << "Closed!" << std::endl;
				//animation.setCurrentFrame(0);
				animation.setIsAnimating(false);
			}
		}
		else if (m_open == 1)// not open
		{
			if (m_timer < 2.00f) // animation has ended
				boxCollider.SetCollisionMode(CollisionMode::e_awake);
			else
			{
				m_open = 2;
				m_timer = 0.0f;
				//sprite.setTexturePath("Resources/Assets/ElaResist.png");
				std::cout << "Open!" << std::endl;
				//setCurrentFrame(0);
				animation.setIsAnimating(true);
			}
		}
		else if (m_open == 2)// not open
		{
			if (m_timer < 0.40f) // animation has ended
				boxCollider.SetCollisionMode(CollisionMode::e_trigger);
			else
			{
				m_open = 3;
				m_timer = 0.0f;
				//sprite.setTexturePath("Resources/Assets/ElaResist.png");
				std::cout << "Open!" << std::endl;
				//animation.setCurrentFrame(0);
				animation.setIsAnimating(false);
			}
		}
		else if (m_open == 3)// not open
		{
			if (m_timer < 2.00f) // animation has ended
				boxCollider.SetCollisionMode(CollisionMode::e_awake);
			else
			{
				m_open = 0;
				m_timer = 0.0f;
				//sprite.setTexturePath("Resources/Assets/ElaResist.png");
				std::cout << "Open!" << std::endl;
				animation.setCurrentFrame(0);
				animation.setIsAnimating(true);
			}
		}
	}
}