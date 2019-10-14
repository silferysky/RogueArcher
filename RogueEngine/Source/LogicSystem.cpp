#include <vector>
#include <map>

#include "Main.h"
#include "BaseSystem.h"
#include "LogicSystem.h"
#include "EventDispatcher.h"
#include "TransformComponent.h"
#include "BoxCollider2DComponent.h"
#include "LogicComponent.h"
#include "Timer.h"

LogicSystem::LogicSystem()
{
	m_entityLogicMap = std::map<Entity, ILogic*>();
}

LogicSystem::~LogicSystem()
{
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
	LISTENER_HANDLER hand = std::bind(&LogicSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_PHYSICSSYSTEM, hand);

	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2DComponent>());
	signature.set(gEngine.m_coordinator.GetComponentType <LogicComponent>());

	gEngine.m_coordinator.SetSystemSignature<LogicSystem>(signature);
}

void LogicSystem::update()
{
	Timer TimerSystem;
	TimerSystem.TimerInit("Logic System");
	for (auto it = m_entityLogicMap.begin(); it != m_entityLogicMap.end(); ++it)
	{
		//Null checker
		if (!it->second)
			continue;

		//Updates the current logic. The individual AI types will handle the state on their own
		it->second->logicUpdate();
	}
	TimerSystem.TimerEnd("Logic System");
}

void LogicSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
	}
	}
}
