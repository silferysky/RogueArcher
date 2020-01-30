#include "Precompiled.h"
#include "ParentChildSystem.h"
#include "Main.h"
#include "ParentEvent.h"
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	ParentChildSystem::ParentChildSystem()
		: System(SystemID::id_PARENTCHILDSYSTEM) {}

	void ParentChildSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PARENTCHILDSYSTEM, ParentChildSystem::Receive);
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		//signature.set(g_engine.m_coordinator.GetComponentType<ChildComponent>());

		g_engine.m_coordinator.SetSystemSignature<ParentChildSystem>(signature);
	}

	void ParentChildSystem::Update()
	{
	}

	void ParentChildSystem::Shutdown()
	{
	}

	void ParentChildSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EvParentSet:
		{
			ParentSetEvent* parentEvent = dynamic_cast<ParentSetEvent*>(ev);
			ReassignParentChildFlags(parentEvent->GetChildEntity(), parentEvent->GetParentEntity());
			break;
		}
		case EvParentTransformUpdate:
		{
			ParentTransformEvent* parentEvent = dynamic_cast<ParentTransformEvent*>(ev);
			HierarchyInfo& parentHierarchyInfo = g_engine.m_coordinator.GetHierarchyInfo(parentEvent->GetParentEntity());
			break;
		}
		case EvParentScaleUpdate:
		{
			ParentScaleEvent* parentEvent = dynamic_cast<ParentScaleEvent*>(ev);
			break;
		}
		case EvParentRotateUpdate:
		{
			ParentRotateEvent* parentEvent = dynamic_cast<ParentRotateEvent*>(ev);
			break;
		}
		}
	}
	bool ParentChildSystem::CheckValidReassign(Entity child, Entity newParent)
	{
		bool isValid = true;
		HierarchyInfo it = g_engine.m_coordinator.GetHierarchyInfo(child);
		while (it.m_parent != -1 && it.m_parent != MAX_ENTITIES)
		{
			if (it.m_parent == child)
			{
				isValid = false;
				break;
			}
			it = g_engine.m_coordinator.GetHierarchyInfo(it.m_parent);
		}

		return isValid;
	}

	void ParentChildSystem::ReassignParentChildFlags(Entity child, Entity newParent)
	{
		//If invalid reassign (loop)
		if (!CheckValidReassign(child, newParent))
			return;

		//This cannot be done directly here, since it is middle of a loop.
		g_engine.m_coordinator.SetReassignParentFlags(child, newParent);
	}
}