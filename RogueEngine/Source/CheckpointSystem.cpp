#include "Precompiled.h"
#include "CheckpointSystem.h"
#include "Main.h"

namespace Rogue
{
	CheckpointSystem::CheckpointSystem()
		: System(SystemID::id_CHECKPOINTSYSTEM)
	{}

	void CheckpointSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_CHECKPOINTSYSTEM, CheckpointSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<CheckpointComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<CheckpointSystem>(signature);
	}

	void CheckpointSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Fade System");

		// For all entities
		for (auto entity : m_entities)
		{
			auto& checkpoint = g_engine.m_coordinator.GetComponent<CheckpointComponent>(entity);



		}

		g_engine.m_coordinator.EndTimeSystem("Fade System");
	}

	void CheckpointSystem::Shutdown()
	{}

	void CheckpointSystem::Receive(Event* ev)
	{
	}
}