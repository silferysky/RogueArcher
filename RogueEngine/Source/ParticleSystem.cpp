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
		signature.set(g_engine.m_coordinator.GetComponentType<ParticleComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		g_engine.m_coordinator.SetSystemSignature<ParticleSystem>(signature);
	}

	void ParticleSystem::Update()
	{
		for (auto entity : m_entities)
		{
			auto& particle = g_engine.m_coordinator.GetComponent<ParticleComponent>(entity);
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			particle.Update(g_deltaTime);

			if (particle.GetLifetime() <= 0)
			{
				g_engine.m_coordinator.DestroyEntity(entity);

				auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
				for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
				{
					if (*iterator == entity)
					{
						activeObjects.erase(iterator);
						break;
					}
				}

				break;
			}
		}
	}

	void ParticleSystem::Receive(Event* ev)
	{
	}

	void ParticleSystem::Shutdown()
	{
	}
}