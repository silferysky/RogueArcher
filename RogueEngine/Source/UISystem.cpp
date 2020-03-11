/* Start Header ************************************************************************/
/*!
\file           UISystem.cpp
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
#include "Precompiled.h"
#include "UISystem.h"
#include "Main.h"
#include "CameraManager.h"

namespace Rogue
{
	UISystem::UISystem()
		: System(SystemID::id_UISYSTEM)
	{}

	void UISystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_UISYSTEM, UISystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<UIComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<UISystem>(signature);

	}

	void UISystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("UI System");
		m_newCameraPos = CameraManager::instance().GetCameraPos();

		m_difference = m_newCameraPos - m_oldCameraPos;

		// For all entities
		for (auto entity : m_entities)
		{
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			Vec2 position = transform.GetPosition();

			position.x += m_difference.x;
			position.y += m_difference.y;

			transform.setPosition(position);
		}

		m_oldCameraPos = m_newCameraPos;

		g_engine.m_coordinator.EndTimeSystem("UI System");
	}

	void UISystem::SetOldCameraPos(const glm::vec3& oldCameraPos)
	{
		m_oldCameraPos = oldCameraPos;
	}

	glm::vec3 UISystem::GetDifference() const
	{
		return m_difference;
	}

	void UISystem::Shutdown()
	{}

	void UISystem::Receive(Event& ev)
	{}
}