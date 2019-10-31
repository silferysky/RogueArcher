#pragma once
#include "REEngine.h"

namespace Rogue
{
	class BoxCollisionSystem :
		public System, public EventListener
	{
	public:
		BoxCollisionSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}