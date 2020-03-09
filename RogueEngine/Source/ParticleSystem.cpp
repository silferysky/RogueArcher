/* Start Header ************************************************************************/
/*!
\file           ParticleSystem.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ParticleSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

		g_engine.m_coordinator.SetSystemSignature<ParticleSystem>(signature);
	}

	void ParticleSystem::Update()
	{
		//g_engine.m_coordinator.InitTimeSystem("Particle System");

		for (auto entity : m_entities)
		{
			auto& particle = g_engine.m_coordinator.GetComponent<ParticleComponent>(entity);

			particle.Update(g_fixedDeltaTime * g_engine.GetTimeScale());

			if (particle.GetLifetime() <= 0)
				g_engine.m_coordinator.AddToDeleteQueue(entity);
		}

		//g_engine.m_coordinator.EndTimeSystem("Particle System");
	}

	void ParticleSystem::Receive(Event& ev)
	{
	}

	void ParticleSystem::Shutdown()
	{
	}
}