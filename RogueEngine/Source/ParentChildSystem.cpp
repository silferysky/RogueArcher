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
}