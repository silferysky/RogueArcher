/* Start Header ************************************************************************/
/*!
\file           MenuControllerSystem.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for MenuControllerSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EventListener.h"
#include "BaseSystem.h"
#include <vector>
#define TRANSITION_TIME 1.0f

namespace Rogue
{
	class MenuControllerSystem
		: public System, public EventListener
	{
		std::vector<Entity> m_menuObjs;
		std::vector<Entity> m_confirmQuitEnt;
		bool m_confirmQuit;
		float m_transitionTimer;
		bool m_isTransitingOut;

	public:

		MenuControllerSystem();
		~MenuControllerSystem();

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event* ev) override;
		void Shutdown() override;

		//Menu UI Control
		void ClearMenuObjs();
		void InitPauseMenu();
		void ToggleControlHelpMenu();
		void ToggleUIMenuObjs();
		void SetUIMenuObjs(bool newActive);
		void ToggleQuitButtonObj();
		size_t GetUIMenuObjsSize();
		void ResumeGame();

		//Transitioning
		void Transit();
		bool GetTransitType() const;
		float GetTransitTime() const;
	};
}

