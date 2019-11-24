#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "ParticleEmitterComponent.h"
#include "TransformComponent.h"

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

		void GenerateExplosion(ParticleEmitterComponent pEmitter, TransformComponent transform);
		//void GenerateSpray(const ParticleEmitterComponent& pEmitter, const TransformComponent& transform);
	};
}
