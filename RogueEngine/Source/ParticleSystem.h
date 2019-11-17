#pragma once
#include "EventListener.h"
#include "BaseSystem.h"

namespace Rogue
{
	class ParticleSystem
		: public System, public EventListener
	{
	public:

		ParticleSystem();
		~ParticleSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event* ev) override;
		void Shutdown() override;
	};
}