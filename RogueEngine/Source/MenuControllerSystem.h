#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include <vector>

namespace Rogue
{
	class MenuControllerSystem
		: public System, public EventListener
	{
		std::vector<Entity> m_menuObjs;

		void ClearMenuObjs();
	public:

		MenuControllerSystem();
		~MenuControllerSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event* ev) override;
		void Shutdown() override;

		void InitPauseMenu();
		void InitControlHelpMenu();
	};
}

