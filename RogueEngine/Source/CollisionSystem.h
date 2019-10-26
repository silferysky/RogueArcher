#pragma once
#include "REEngine.h"

namespace Rogue
{
	class CollisionSystem :
		public System, public EventListener
	{
		CollisionManager m_colliderManager;
	public:
		CollisionSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}
