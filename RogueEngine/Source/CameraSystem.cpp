#include "CameraSystem.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include "GraphicsEvent.h"

namespace Rogue
{
	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//

	CameraSystem::CameraSystem()
		: System(SystemID::id_CAMERASYSTEM), m_worldCamera{ false },
		m_cameraPos{ 0.0f, 0.0f, 0.0f }, m_worldUp{ 0.0f, 1.0f, 0.0f },
		m_cameraFront{ 0.0f, 0.0f, -1.0f }, m_cameraShake{}, m_cameraMin{ -1600, -1600 }, m_cameraMax{ 1600, 1600 }
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

	glm::mat4 CameraSystem::GetViewMatrix()
	{
		return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}

	void CameraSystem::ResetCamera()
	{
		m_cameraPos = { 0.0f, 0.0f, 0.0f };
	}

	void CameraSystem::ToggleWorldCamera()
	{
		m_worldCamera = !m_worldCamera;

		// Reset camera position to center
		if (m_worldCamera)
			ResetCamera();
	}

	CameraShake CameraSystem::GetShake() const
	{
		return m_cameraShake;
	}

	Vec2 CameraSystem::GetCameraMin() const
	{
		return m_cameraMin;
	}

	Vec2 CameraSystem::GetCameraMax() const
	{
		return m_cameraMax;
	}

	bool CameraSystem::GetWorldCamera() const
	{
		return m_worldCamera;
	}

	void CameraSystem::SetWorldCamera(const bool& camera)
	{
		m_worldCamera = camera;
	}

	glm::vec3 CameraSystem::GetCameraPos() const
	{
		return m_cameraPos;
	}

	void CameraSystem::SetCameraMin(const Vec2& cameraMin)
	{
		m_cameraMin = cameraMin;
	}

	void CameraSystem::SetCameraMax(const Vec2& cameraMax)
	{
		m_cameraMax = cameraMax;
	}

	void CameraSystem::SetCameraPos(const glm::vec3& position)
	{
		m_cameraPos = position;
	}

	void CameraSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Camera System");

		m_cameraShake.Update();
		auto shakeOffset = m_cameraShake.getOffset();
		
		if (!m_worldCamera)
		{
			// For all entities
			for (auto entity : m_entities)
			{
				if (g_engine.m_coordinator.GetComponent<CameraComponent>(entity).getIsActive())
				{
					auto transformPos = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).getPosition();

					if (transformPos.x > m_cameraMax.x)
						transformPos.x = m_cameraMax.x;

					if (transformPos.x < m_cameraMin.x)
						transformPos.x = m_cameraMin.x;

					if (transformPos.y > m_cameraMax.y)
						transformPos.y = m_cameraMax.y;

					if (transformPos.y < m_cameraMin.y)
						transformPos.y = m_cameraMin.y;

					m_cameraPos = glm::vec3(transformPos.x + shakeOffset.x, transformPos.y + shakeOffset.y, 0.0f);

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

			if (keycode == KeyPress::KeyShift)
				ToggleWorldCamera();

			return;
		} //End KeyTriggered
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
			KeyPress keycode = EvPressKey->GetKeyCode();

			if (keycode == KeyPress::KeyF1)
				g_engine.ZoomIn();

			if (keycode == KeyPress::KeyF2)
				g_engine.ZoomOut();

			return;
		} //End KeyPressed
		}
	}

	void CameraSystem::Shutdown()
	{}
}