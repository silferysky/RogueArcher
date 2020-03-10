/* Start Header ************************************************************************/
/*!
\file           UISystem.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for UISystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "glm.hpp"

namespace Rogue
{
	class UISystem : public System, public EventListener
	{
		glm::vec3 m_oldCameraPos;
		glm::vec3 m_newCameraPos;

		glm::vec3 m_difference;
	public:
		UISystem();
		~UISystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void Receive(Event& ev);

		void SetOldCameraPos(const glm::vec3& oldCameraPos);
		glm::vec3 GetDifference() const;
	};
}