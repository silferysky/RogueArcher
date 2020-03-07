#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include "TileMapComponent.h"

namespace Rogue
{
	class TileMapSystem :
		public System, public EventListener
	{
	public:
		// Basic System 
		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;
	};
}