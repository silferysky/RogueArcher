#include "Precompiled.h"
#include "FadeSystem.h"
#include "Main.h"

namespace Rogue
{
	FadeSystem::FadeSystem()
		: System(SystemID::id_MASKINGSYSTEM)
	{}

	void FadeSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_MASKINGSYSTEM, FadeSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<SpriteComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<MaskingComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<FadeSystem>(signature);
	}

	void FadeSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Masking System");

		// For all entities
		for (auto entity : m_entities)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		}

		g_engine.m_coordinator.EndTimeSystem("Masking System");
	}

	void FadeSystem::Shutdown()
	{}

	void FadeSystem::Receive(Event* ev)
	{}
}