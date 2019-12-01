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

static float RandFloat(const float& lowerLimit, const float& upperLimit)
{
	return lowerLimit + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upperLimit - lowerLimit)));
}

namespace Rogue
{
	class ParticleEmitterSystem
		: public System, public EventListener
	{
		Entity unitParticle;
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
