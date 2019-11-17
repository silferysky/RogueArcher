#pragma once
#include "EventListener.h"
#include "BaseSystem.h"

namespace Rogue
{
	class MenuControllerSystem
		: public System, public EventListener
	{
	public:

		MenuControllerSystem();
		~MenuControllerSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event* ev) override;
		void Shutdown() override;
	};
}

