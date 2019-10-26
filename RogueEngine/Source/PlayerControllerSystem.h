#pragma once
#include "EventListener.h"
#include "BaseSystem.h"

namespace Rogue
{
	class PlayerControllerSystem
		: public System, public EventListener
	{
	public:

		PlayerControllerSystem();
		~PlayerControllerSystem() = default;

		//Basic System 
		void init() override;
		void update() override;
		void receive(Event * ev) override;

	private:

	};
}