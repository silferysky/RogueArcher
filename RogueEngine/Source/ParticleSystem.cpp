#include "Precompiled.h"
#include "ParticleSystem.h"
#include "Main.h"


namespace Rogue
{
	ParticleSystem::ParticleSystem()
		:System(SystemID::id_PARTICLESYSTEM)
	{
	}

	void ParticleSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PARTICLESYSTEM, ParticleSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		g_engine.m_coordinator.SetSystemSignature<ParticleSystem>(signature);
	}

	void ParticleSystem::Update()
	{

	}
	void ParticleSystem::Receive(Event* ev)
	{
	}

	void ParticleSystem::Shutdown()
	{
	}
}