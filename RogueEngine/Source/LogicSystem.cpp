/* Start Header ************************************************************************/
/*!
\file           LogicSystem.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for LogicSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"

#include "Main.h"
#include "LogicSystem.h"
#include "EventDispatcher.h"
#include "GameEvent.h"
#include "KeyEvent.h"
#include "Timer.h"

//AI Types
#include "FinderAI.h"
#include "PlatformAI.h" //Also includes PatrolAI
#include "TriggerAI.h"
#include "TransitionObject.h"
#include "TeleCharge.h"

namespace Rogue
{
	LogicSystem::LogicSystem()
		: System(SystemID::id_LOGICSYSTEM)
	{
		m_entityLogicMap = std::map<Entity, std::vector<std::shared_ptr<ScriptComponent>>>();
	}


	void LogicSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_LOGICSYSTEM, LogicSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<LogicComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<StatsComponent>());

		g_engine.m_coordinator.SetSystemSignature<LogicSystem>(signature);
	}

	void LogicSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Logic System");

		//Check if any new AI needs to be added
		if (m_entities.size() > m_entityLogicMap.size())
		{
			AddExcessAI();
		}
		else if (m_entities.size() < m_entityLogicMap.size()) //Check if any AI needs to be deleted
		{
			RemoveExcessAI();
		}

		//Logic system specifically only wants to update AddExcessAI and RemoveExcessAI even while game is not running or game is paused
		if (!g_engine.m_coordinator.GetGameState() || g_engine.m_coordinator.GetPauseState())
		{
			g_engine.m_coordinator.EndTimeSystem("Logic System");
			return;
		}

		//Pre AI Update

		//Reset all LogicComponent's active state bit
		for (std::pair<Entity, std::vector<std::shared_ptr<ScriptComponent>>> it : m_entityLogicMap)
		{
			for (std::shared_ptr<ScriptComponent> ai : it.second)
				ai->GetLogicComponent()->ResetActiveStateBit();
		}

		//Update all active state bits
		for (std::pair<Entity, std::vector<std::shared_ptr<ScriptComponent>>> it : m_entityLogicMap)
		{
			for (std::shared_ptr<ScriptComponent> ai : it.second)
				ai->AIActiveStateUpdate();
		}

		//AI Update
		for (std::pair<Entity, std::vector<std::shared_ptr<ScriptComponent>>> it : m_entityLogicMap)
		{
			for (std::shared_ptr<ScriptComponent> ai : it.second)
				ai->LogicUpdate();
		}
		g_engine.m_coordinator.EndTimeSystem("Logic System");
	}

	void LogicSystem::Receive(Event* ev)
	{
		switch (ev->GetEventCat())
		{
		case EventCategory::EventCatCollision:
		case EventCategory::EventCatTrigger:
		{
			EntCollisionOrTrigger* event = dynamic_cast<EntCollisionOrTrigger*>(ev);
			Entity object = event->GetEntityID();
			Entity triggered = event->GetOtherEntity();
			for (Entity m : m_entities)
			{
				if (m == object || m == triggered)
					if (m_entityLogicMap[m].size())
					{
						for (auto& ai : m_entityLogicMap[m])
						{
							ai->HandleCollision(event);
							if (!m_entityLogicMap[m].size())
								return;
						}
					}
			}
			return;
		}
		}
	}

	void LogicSystem::AddLogicInterface(Entity entity, std::shared_ptr<ScriptComponent> logicInterface)
	{
		m_entityLogicMap[entity].push_back(logicInterface);
		//m_entityLogicMap.emplace(std::pair < Entity, std::shared_ptr<BaseAI>>(entity, logicInterface));
		//m_entityLogicMap.insert({ entity, logicInterface });
	}

	void LogicSystem::RemoveLogicInterface(Entity entity)
	{
		m_entityLogicMap.erase(entity);
	}

	void LogicSystem::ClearLogicInterface()
	{
		for (auto& elements : m_entityLogicMap)
		{
			elements.second.clear();
		}

		m_entityLogicMap.clear();
	}

	void LogicSystem::AddExcessAI()
	{
		for (auto& entities : m_entities)
		{
			if (!m_entityLogicMap[entities].size())
			{
				//Logic component will exist if it is in m_entities
				auto& logicComponent = g_engine.m_coordinator.GetComponent<LogicComponent>(entities);
				auto& statsComponent = g_engine.m_coordinator.GetComponent<StatsComponent>(entities);

				switch (logicComponent.GetLogicType())
				{
				case AIType::AI_Finder:
				{
					FinderAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<FinderAI>(newAI));
					break;
				}
				case AIType::AI_Patrol:
				{
					PatrolAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<PatrolAI>(newAI));
					break;
				}
				case AIType::AI_Trigger:
				{
					TriggerAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TriggerAI>(newAI));
					break;
				}
				case AIType::AI_Platform:
				{
					PlatformAI newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<PlatformAI>(newAI));
					break;
				}
				case AIType::Obj_Transition:
				{
					TransitionObject newAI(entities, logicComponent, statsComponent, statsComponent.GetTransitionLevel());
					AddLogicInterface(entities, std::make_shared<TransitionObject>(newAI));
					break;
				}
				case AIType::UI_TeleCharge:
				{
					TeleCharge newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<TeleCharge>(newAI));
					break;
				}
				case AIType::AI_Static:
				default:
				{
					ScriptComponent newAI(entities, logicComponent, statsComponent);
					AddLogicInterface(entities, std::make_shared<ScriptComponent>(newAI));
					break;
				}
				}
			}
		}
	}

	void LogicSystem::RemoveExcessAI()
	{
		for (auto entityLogicIt = m_entityLogicMap.begin(); entityLogicIt != m_entityLogicMap.end(); ++entityLogicIt)
		{
			auto it = m_entities.find(entityLogicIt->first);
			if (it == m_entities.end())
			{
				entityLogicIt = m_entityLogicMap.erase(entityLogicIt);
			}

			if (!m_entityLogicMap.size())
				break;
		}
	}

	void LogicSystem::GenerateScript()
	{
		for (auto& entity : m_entities)
		{
			auto& logicComponent = g_engine.m_coordinator.GetComponent<LogicComponent>(entity);
			auto& statsComponent = g_engine.m_coordinator.GetComponent<StatsComponent>(entity);
			auto scriptStringVector = logicComponent.GetScriptString();

			if (!scriptStringVector.size())
				continue;

			for (auto& scriptString : scriptStringVector)
			{
				switch (logicComponent.GetLogicType())
				{
				case AIType::AI_Finder:
				{
					FinderAI newAI(entity, logicComponent, statsComponent);
					AddLogicInterface(entity, std::make_shared<FinderAI>(newAI));
					break;
				}
				case AIType::AI_Patrol:
				{
					PatrolAI newAI(entity, logicComponent, statsComponent);
					AddLogicInterface(entity, std::make_shared<PatrolAI>(newAI));
					break;
				}
				case AIType::AI_Trigger:
				{
					TriggerAI newAI(entity, logicComponent, statsComponent);
					AddLogicInterface(entity, std::make_shared<TriggerAI>(newAI));
					break;
				}
				case AIType::AI_Platform:
				{
					PlatformAI newAI(entity, logicComponent, statsComponent);
					AddLogicInterface(entity, std::make_shared<PlatformAI>(newAI));
					break;
				}
				case AIType::Obj_Transition:
				{
					TransitionObject newAI(entity, logicComponent, statsComponent, statsComponent.GetTransitionLevel());
					AddLogicInterface(entity, std::make_shared<TransitionObject>(newAI));
					break;
				}
				case AIType::UI_TeleCharge:
				{
					TeleCharge newAI(entity, logicComponent, statsComponent);
					AddLogicInterface(entity, std::make_shared<TeleCharge>(newAI));
					break;
				}
				case AIType::AI_Static:
				default:
				{
					ScriptComponent newAI(entity, logicComponent, statsComponent);
					AddLogicInterface(entity, std::make_shared<ScriptComponent>(newAI));
					break;
				}
				}
			}
		}
	}

	void LogicSystem::DeleteScript()
	{
		for (auto& scripts : m_entityLogicMap)
		{
			scripts.second.clear();
		}
		m_entityLogicMap.clear();
	}

	void LogicSystem::TriggerNextDoor()
	{
		for (Entity entity : m_entities)
		{
			StatsComponent& stats = g_engine.m_coordinator.GetComponent<StatsComponent>(entity);

			//If not patrolling, set patrolling
			if (!stats.GetIsPatrolling())
			{
				(*m_entityLogicMap[entity].begin())->GetStatsComponent()->SetIsPatrolling(true);
				stats.SetIsPatrolling(true);
				if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(entity))
					g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity).SetCollisionMode(CollisionMode::e_trigger);
				break;
			}
		}
	}

	/*void LogicSystem::SeekNearestWaypoint(Entity ent)
	{
		Vec2* currentLocation = &g_engine.m_coordinator.GetComponent<TransformComponent>(ent).getPosition();
		Vec2* nearestWaypoint = nullptr;
		float distance = 0.0f;
		for (std::vector<Vec2>::iterator it = m_entityLogicMap[ent]->GetWaypoints().begin(); it != m_entityLogicMap[ent]->GetWaypoints().end(); ++it)
		{
			//First iteration sets first value by default
			if (nearestWaypoint == nullptr)
			{
				nearestWaypoint = &*it;
				distance = Vec2Distance(*currentLocation, *it);
				continue;
			}

			//For all other iterations
			float tempDistance = Vec2Distance(*currentLocation, *it);
			if (tempDistance < distance)
			{
				nearestWaypoint = &*it;
				distance = tempDistance;
			}
		}

		//Null checker
		if (!nearestWaypoint)
			return;
		
		//Sets the best waypoint into the new location
		m_entityLogicMap[ent]->AddNextPoint(*nearestWaypoint);
	}*/

	void LogicSystem::CreateMoveEvent(Entity ent, Vec2 vec)
	{
		EntMoveEvent* moveEvent = new EntMoveEvent(ent, false, vec);
		moveEvent->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		EventDispatcher::instance().AddEvent(moveEvent);
	}

	void LogicSystem::Shutdown()
	{}

}