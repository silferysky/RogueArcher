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

	void LogicSystem::init()
	{
		REGISTER_LISTENER(SystemID::id_LOGICSYSTEM, LogicSystem::receive);

		Signature signature;
		signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
		signature.set(gEngine.m_coordinator.GetComponentType<LogicComponent>());

		gEngine.m_coordinator.SetSystemSignature<LogicSystem>(signature);
	}

	void LogicSystem::update()
	{
		gEngine.m_coordinator.InitTimeSystem("Logic System");
		for (auto it = m_entityLogicMap.begin(); it != m_entityLogicMap.end(); ++it)
		{
			//Null checker
			if (!it->second)
				continue;

			//Updates the current logic. The individual AI types will handle the state on their own
			it->second->logicUpdate();
		}
		gEngine.m_coordinator.EndTimeSystem("Logic System");
	}

	void LogicSystem::receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* keypressevent = dynamic_cast<KeyPressEvent*>(ev);

			KeyPress keycode = keypressevent->GetKeyCode();

			if (keycode == KeyPress::KeyEsc)
				gEngine.SetGameIsRunning(false);
		}
		}
	}

}