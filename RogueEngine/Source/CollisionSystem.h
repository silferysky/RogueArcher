#pragma once
#include "REEngine.h"
#include "CollisionManager.h"
#include "EditorHierarchyInfo.h"
namespace Rogue
{
	class CollisionSystem :
		public System, public EventListener
	{
	public:
		CollisionSystem();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}
#pragma once
