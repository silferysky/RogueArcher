#include "Precompiled.h"
#include "ParentChildSystem.h"
#include "ParentEvent.h"

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