#pragma once
#include "REEngine.h"

namespace Rogue
{
	class CircleCollisionSystem :
		public System, public EventListener
	{
	public:
		CircleCollisionSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}