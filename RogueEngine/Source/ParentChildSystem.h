#pragma once
#include "BaseSystem.h"
#include "EventListener.h"

namespace Rogue
{
	class ParentChildSystem
		: public System, public EventListener
	{
	public:
		ParentChildSystem();
		~ParentChildSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}