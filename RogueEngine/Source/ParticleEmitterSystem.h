#pragma once
#include "EventListener.h"
#include "BaseSystem.h"

static float RandFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

static float RandFloat(const float& limit)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / limit);
}

namespace Rogue
{
	class ParticleEmitterSystem
		: public System, public EventListener
	{
	public:
		ParticleEmitterSystem();
		~ParticleEmitterSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event* ev) override;
		void Shutdown() override;

		void GenerateParticles(const Entity& entity);
	};
}
