#include <vector>
#include <map>

#include "Main.h"
#include "BaseSystem.h"
#include "LogicSystem.h"
#include "EventDispatcher.h"
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
		m_entityLogicMap = std::map<Entity, ILogic*>();
	}

	void LogicSystem::AddLogicInterface(Entity entity, ILogic* logicInterface)
	{
		m_entityLogicMap.insert({ entity, logicInterface });
	}

	void LogicSystem::RemoveLogicInterface(Entity entity)
	{
		m_entityLogicMap.erase(entity);
	}

	void LogicSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_LOGICSYSTEM, LogicSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<LogicComponent>());

		g_engine.m_coordinator.SetSystemSignature<LogicSystem>(signature);
	}

	void LogicSystem::Update()
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

	void LogicSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* keypressevent = dynamic_cast<KeyPressEvent*>(ev);

			KeyPress keycode = keypressevent->GetKeyCode();

			if (keycode == KeyPress::KeyEsc)
				g_engine.SetGameIsRunning(false);

			return;
		}
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keyTriggeredEvent = dynamic_cast<KeyTriggeredEvent*>(ev);
			auto keycode = keyTriggeredEvent->GetKeyCode();

			if (keycode == KeyPress::KeyC)
			{
				g_engine.m_coordinator.clone(1);
			}

			return;
		}
		}
	}

	void LogicSystem::Shutdown()
	{}

}