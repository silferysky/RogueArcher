#pragma once
#include "REEngine.h"
#include "CollisionManager.h"

namespace Rogue
{
	class CollisionSystem :
		public System, public EventListener
	{
	public:
		static CollisionManager s_collisionManager;

		CollisionSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}
#pragma once
