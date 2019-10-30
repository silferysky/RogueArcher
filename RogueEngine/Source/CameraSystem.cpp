#include "CameraSystem.h"
#include "Main.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"

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

	void CameraSystem::SetCameraMin(const float& x, const float& y)
	{
		m_cameraMin = { x, y };
	}

	void CameraSystem::SetCameraMax(const float& x, const float& y)
	{
		m_cameraMin = { x, y };
	}

	void CameraSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Camera System");

		m_cameraShake.Update(g_deltaTime);
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
		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (keycode == KeyPress::KeyP)
				m_cameraShake.SetShake(13.0f);

			if (keycode == KeyPress::KeyShift)
				ToggleWorldCamera();

			return;
		} //End KeyTriggered
		}
	}

	void CameraSystem::Shutdown()
	{}
}