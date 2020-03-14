#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "AppEvent.h"

namespace Rogue
{
	class SoulSystem :
		public System, public EventListener
	{
	public:
		// Constructor
		SoulSystem();

		// System overrides
		void Init() override;
		void TrueInit();
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;
	};
}