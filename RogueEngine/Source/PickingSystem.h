#pragma once
#include "BaseSystem.h"
#include "EventListener.h"

namespace Rogue
{
	class PickingSystem : public System, public EventListener
	{
	public:
		PickingSystem();
		~PickingSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}