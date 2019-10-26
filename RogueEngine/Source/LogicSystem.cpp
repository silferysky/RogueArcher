#include <vector>
#include <map>

#include "Main.h"
#include "BaseSystem.h"
#include "LogicSystem.h"
#include "EventDispatcher.h"
#include "GameEvent.h"
#include "KeyEvent.h"
#include "TransformComponent.h"
#include "BoxCollider2DComponent.h"
#include "LogicComponent.h"
#include "Timer.h"

namespace Rogue
{
	LogicSystem::LogicSystem()
		: System(SystemID::id_LOGICSYSTEM)
	{
		m_entityLogicMap = std::map<Entity, BaseAI*>();
	}

	void LogicSystem::AddLogicInterface(Entity entity, BaseAI* logicInterface)
	{
		m_entityLogicMap.insert({ entity, logicInterface });
	}

	void LogicSystem::RemoveLogicInterface(Entity entity)
	{
		m_entityLogicMap.erase(entity);
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


	void LogicSystem::init()
	{
		REGISTER_LISTENER(SystemID::id_LOGICSYSTEM, LogicSystem::receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<LogicComponent>());

		g_engine.m_coordinator.SetSystemSignature<LogicSystem>(signature);
	}

	void LogicSystem::update()
	{
		g_engine.m_coordinator.InitTimeSystem("Logic System");
		for (auto it = m_entityLogicMap.begin(); it != m_entityLogicMap.end(); ++it)
		{
			//Null checker
			if (!it->second)
				continue;


			//Updates the current logic. The individual AI types will handle the state on their own
			it->second->logicUpdate();
		}
		g_engine.m_coordinator.EndTimeSystem("Logic System");
	}

	void LogicSystem::receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);

			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (keycode == KeyPress::KeyEsc)
				g_engine.SetGameIsRunning(false);

			
			if (keycode == KeyPress::KeyC)
			{
				g_engine.m_coordinator.clone(1);
			}
		}
		}
	}

}