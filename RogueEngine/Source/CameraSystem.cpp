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
#include "PickingManager.h"

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

	void CameraSystem::setIsActive(const bool& isActive)
	{
		m_isActive = isActive;
	}

	bool CameraSystem::getIsActive() const
	{
		return m_isActive;
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

					if (m_isActive)
					{
						float deltaX = transformPos.x - position.x + cameraOffset.x;
						float deltaY = transformPos.y - position.y + cameraOffset.y;

						//if (g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entity).m_grounded)
						//{
							// Do not move the camera for too small distances
						if (deltaX > 0.5 || deltaX < -0.5)
							position.x += static_cast<int>(deltaX)* m_cameraLerp* g_deltaTime;

						if (deltaY > 0.5 || deltaY < -0.5)
							position.y += static_cast<int>(deltaY)* m_cameraLerp* g_deltaTime;
						//}
					}
					else
					{
						position = glm::vec3(position.x + shakeOffset.x, position.y + shakeOffset.y, 0.0f);
					}

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

			//if (keycode == KeyPress::KeyShift && g_engine.GetIsFocused())
				//ToggleWorldCamera();

			return;
		} //End KeyTriggered
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
			KeyPress keycode = EvPressKey->GetKeyCode();

			if (g_engine.GetIsFocused() && g_engine.m_coordinator.GetEditorIsRunning())
			{
				if (keycode == KeyPress::MB3 && PickingManager::instance().isCursorinViewPort())
				{
					glm::vec3 cursorPos{ PickingManager::instance().GetWorldCursor().x, PickingManager::instance().GetWorldCursor().y, 0 };
					CameraManager::instance().SetCameraPos(cursorPos);
				}

				if (keycode == KeyPress::KeyF1)
					CameraManager::instance().ZoomIn();

				if (keycode == KeyPress::KeyF2)
					CameraManager::instance().ZoomOut();

				if (keycode == KeyPress::KeyArrowLeft)
				{
					auto& cameraManager = CameraManager::instance();
					auto cameraPos = cameraManager.GetCameraPos();

					cameraPos.x -= 10.0f;

					cameraManager.SetCameraPos(cameraPos);
				}

				if (keycode == KeyPress::KeyArrowRight)
				{
					auto& cameraManager = CameraManager::instance();
					auto cameraPos = cameraManager.GetCameraPos();

					cameraPos.x += 10.0f;

					cameraManager.SetCameraPos(cameraPos);
				}

				if (keycode == KeyPress::KeyArrowUp)
				{
					auto& cameraManager = CameraManager::instance();
					auto cameraPos = cameraManager.GetCameraPos();

					cameraPos.y += 10.0f;

					cameraManager.SetCameraPos(cameraPos);
				}

				if (keycode == KeyPress::KeyArrowDown)
				{
					auto& cameraManager = CameraManager::instance();
					auto cameraPos = cameraManager.GetCameraPos();

					cameraPos.y -= 10.0f;

					cameraManager.SetCameraPos(cameraPos);
				}
			}
			return;
		} //End KeyPressed
		}
	}

	void CameraSystem::Shutdown()
	{}
}