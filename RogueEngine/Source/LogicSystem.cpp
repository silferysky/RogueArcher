#include <vector>
#include <map>

#include "Main.h"
#include "LogicSystem.h"
#include "EventDispatcher.h"
#include "GameEvent.h"
#include "KeyEvent.h"
#include "TransformComponent.h"
#include "BoxCollider2DComponent.h"
#include "LogicComponent.h"
#include "Timer.h"

//AI Types
#include "FinderAI.h"

namespace Rogue
{
	LogicSystem::LogicSystem()
		: System(SystemID::id_LOGICSYSTEM)
	{
		m_entityLogicMap = std::map<Entity, std::shared_ptr<BaseAI>>();
	}


	void LogicSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_LOGICSYSTEM, LogicSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<LogicComponent>());

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
		if (!g_engine.m_coordinator.GetGameState() || g_engine.m_coordinator.GetPauseState)
		{
			g_engine.m_coordinator.EndTimeSystem("Logic System");
			return;
		}

		for (auto it = m_entityLogicMap.begin(); it != m_entityLogicMap.end(); ++it)
		{
			//Null checker
			if (!it->second)
				continue;

			//Converts the shared_ptr into correct type based on ID
			switch (g_engine.m_coordinator.GetComponent<LogicComponent>(it->first).GetLogicType())
			{
			case AIType::AI_Finder:
			{
				std::dynamic_pointer_cast<FinderAI>(it->second)->LogicUpdate();
				break;
			}
			case AIType::AI_Static:
			default:
			{
				it->second->LogicUpdate();
			}
			}

			//Updates the current logic. The individual AI types will handle the state on their own
			//it->second->LogicUpdate();
		}
		g_engine.m_coordinator.EndTimeSystem("Logic System");
	}

	void LogicSystem::Receive(Event* ev)
	{
	}

	void LogicSystem::AddLogicInterface(Entity entity, std::shared_ptr<BaseAI> logicInterface)
	{
		m_entityLogicMap[entity] = logicInterface;
		//m_entityLogicMap.emplace(std::pair < Entity, std::shared_ptr<BaseAI>>(entity, logicInterface));
		//m_entityLogicMap.insert({ entity, logicInterface });
	}

	void LogicSystem::RemoveLogicInterface(Entity entity)
	{
		m_entityLogicMap.erase(entity);
	}

	void LogicSystem::ClearLogicInterface()
	{
		m_entityLogicMap.clear();
	}

	void LogicSystem::RemoveExcessAI()
	{
		for (auto entityLogicIt = m_entityLogicMap.begin(); entityLogicIt != m_entityLogicMap.end(); ++entityLogicIt)
		{
			auto it = m_entities.find(entityLogicIt->first);
			if (it == m_entities.end())
			{
				m_entityLogicMap.erase(entityLogicIt);
			}

			if (!m_entityLogicMap.size())
				break;
		}
	}

	void LogicSystem::AddExcessAI()
	{
		for (auto& entities : m_entities)
		{
			if (m_entityLogicMap[entities] == nullptr)
			{
				//Logic component will exist if it is in m_entities
				auto& logicComponent = g_engine.m_coordinator.GetComponent<LogicComponent>(entities);

				switch (logicComponent.GetLogicType())
				{
				case AIType::AI_Finder:
				{
					FinderAI newAI(entities, logicComponent);
					AddLogicInterface(entities, std::make_shared<FinderAI>(newAI));
					break;
				}
				case AIType::AI_Patrol:
				{
					BaseAI newAI(entities, logicComponent);
					AddLogicInterface(entities, std::make_shared<BaseAI>(newAI));
					break;
				}
				case AIType::AI_Static:
				default:
				{
					BaseAI newAI(entities, logicComponent);
					AddLogicInterface(entities, std::make_shared<BaseAI>(newAI));
					break;
				}
				}
			}
		}
	}

	void LogicSystem::SeekNearestWaypoint(Entity ent)
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
	}

	void LogicSystem::CreateMoveEvent(Entity ent, Vec2 vec)
	{
		EntMoveEvent* moveEvent = new EntMoveEvent(ent, false, vec);
		moveEvent->SetSystemReceivers((int)SystemID::id_PHYSICSSYSTEM);
		EventDispatcher::instance().AddEvent(moveEvent);
	}

	void LogicSystem::Shutdown()
	{}

}