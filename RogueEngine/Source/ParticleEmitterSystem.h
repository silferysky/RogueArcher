#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "ParticleEmitterComponent.h"
#include "TransformComponent.h"

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

		void GenerateExplosion(const Entity& entity);
		void GenerateSpray(const Entity& entity);
		void GenerateTrail(const Entity& entity);
	};
}
