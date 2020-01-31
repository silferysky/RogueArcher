/* Start Header ************************************************************************/
/*!
\file           CameraSystem.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CameraSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CameraSystem.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include "GraphicsEvent.h"
#include "CameraManager.h"

namespace Rogue
{
	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//

	CameraSystem::CameraSystem()
		: System(SystemID::id_CAMERASYSTEM), m_worldCamera{ false }, m_worldUp{ 0.0f, 1.0f, 0.0f },
		m_cameraFront{ 0.0f, 0.0f, -1.0f }, m_cameraUp{}, m_cameraRight{},
		m_cameraShake{}
	{}

	void CameraSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_CAMERASYSTEM, CameraSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<CameraComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set system signature.
		g_engine.m_coordinator.SetSystemSignature<CameraSystem>(signature);

		m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
		m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
	}

	glm::mat4 CameraSystem::GetViewMatrix(const float& parallax)
	{
		glm::vec3 cameraPos = CameraManager::instance().GetCameraPos();
		return glm::lookAt({ cameraPos.x * parallax, cameraPos.y, cameraPos.z },
			glm::vec3{ cameraPos.x * parallax, cameraPos.y, cameraPos.z } + m_cameraFront, m_cameraUp);
	}

	void CameraSystem::ResetCamera()
	{
		CameraManager::instance().SetCameraPos({ 0.0f, 0.0f, 0.0f });
	}

	void CameraSystem::ToggleWorldCamera()
	{
		m_worldCamera = !m_worldCamera;

		// Reset camera position to center
		if (m_worldCamera)
			ResetCamera();
	}

	glm::vec3 CameraSystem::GetCameraFront() const
	{
		return m_cameraFront;
	}

	glm::vec3 CameraSystem::GetCameraUp() const
	{
		return m_cameraUp;
	}

	CameraShake CameraSystem::GetShake() const
	{
		return m_cameraShake;
	}

	float CameraSystem::GetLerp() const
	{
		return m_cameraLerp;
	}

	bool CameraSystem::GetWorldCamera() const
	{
		return m_worldCamera;
	}

	void CameraSystem::SetWorldCamera(const bool& camera)
	{
		m_worldCamera = camera;
	}

	void CameraSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Camera System");

		m_cameraShake.Update();
		auto shakeOffset = m_cameraShake.getOffset();
		
		if (!m_worldCamera && g_engine.m_coordinator.GameIsActive())
		{
			// For all entities
			for (auto entity : m_entities)
			{
				if (g_engine.m_coordinator.GetComponent<CameraComponent>(entity).getIsActive())
				{
					Vec2 transformPos = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).GetPosition();
					Vec2 cameraOffset = g_engine.m_coordinator.GetComponent<CameraComponent>(entity).getOffset();
					Vec2 cameraMin = CameraManager::instance().GetCameraMin();
					Vec2 cameraMax = CameraManager::instance().GetCameraMax();

					// ensure camera doesnt go out of bounds
					if (transformPos.x > cameraMax.x)
						transformPos.x = cameraMax.x;

					if (transformPos.x < cameraMin.x)
						transformPos.x = cameraMin.x;

					if (transformPos.y > cameraMax.y)
						transformPos.y = cameraMax.y;

					if (transformPos.y < cameraMin.y)
						transformPos.y = cameraMin.y;

					//m_target = transformPos;

					// For camera panning
					glm::vec3 position = CameraManager::instance().GetCameraPos();

					int deltaX = static_cast<int>(transformPos.x - position.x + shakeOffset.x + cameraOffset.x);
					int deltaY = static_cast<int>(transformPos.y - position.y + shakeOffset.y + cameraOffset.y);

					// Do not move the camera for too small distances
					if (deltaX > 1)
						position.x += deltaX * m_cameraLerp * g_deltaTime;

					if (deltaY > 1)
						position.y += deltaY * m_cameraLerp * g_deltaTime;

					CameraManager::instance().SetCameraPos(position);
					//glm::vec3(transformPos.x + shakeOffset.x, transformPos.y + shakeOffset.y, 0.0f);

					break;
				}
			}
		}

		g_engine.m_coordinator.EndTimeSystem("Camera System");
	}

	void CameraSystem::Receive(Event* ev)
	{

		switch (ev->GetEventType())
		{
		case EventType::EvCameraShake:
		{
			CameraShakeEvent* cameraShakeEvent = dynamic_cast<CameraShakeEvent*>(ev);
			m_cameraShake.SetShake(cameraShakeEvent->getMagnitude());
			return;
		}
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			//if (keycode == KeyPress::Numpad9)
			//	m_cameraShake.SetShake(13.0f);

			if (keycode == KeyPress::KeyShift && g_engine.GetIsFocused())
				ToggleWorldCamera();

			return;
		} //End KeyTriggered
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
			KeyPress keycode = EvPressKey->GetKeyCode();

			if (g_engine.GetIsFocused())
			{
				if (keycode == KeyPress::KeyF1)
					CameraManager::instance().ZoomIn();

				if (keycode == KeyPress::KeyF2)
					CameraManager::instance().ZoomOut();
			}
			return;
		} //End KeyPressed
		}
	}

	void CameraSystem::Shutdown()
	{}
}