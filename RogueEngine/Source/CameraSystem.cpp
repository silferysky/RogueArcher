#include "CameraSystem.h"
#include "Main.h"
#include "EventDispatcher.h"

namespace Rogue
{
	//-------------------------------------------------------//
	//              PUBLIC MEMBER FUNCTIONS					 //
	//-------------------------------------------------------//

	CameraSystem::CameraSystem()
		: System(SystemID::id_CAMERASYSTEM) {}

	void CameraSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_CAMERASYSTEM, CameraSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<CameraComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set system signature.
		g_engine.m_coordinator.SetSystemSignature<CameraSystem>(signature);

		m_cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
		m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);

		UpdateVectors();
	}

	glm::mat4 CameraSystem::GetViewMatrix()
	{
		return glm::lookAt(m_cameraPos, m_cameraPos + m_cameraFront, m_cameraUp);
	}

	void CameraSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Camera System");
		
		// For all entities
		for (auto entity : m_entities)
		{
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			m_cameraPos = glm::vec3(transform.getPosition().x, transform.getPosition().y, 0.0f);
		}

		g_engine.m_coordinator.EndTimeSystem("Camera System");
	}

	void CameraSystem::UpdateVectors()
	{
		// Re-calculate the Right and Up vector
		m_cameraRight = glm::normalize(glm::cross(m_cameraFront, m_worldUp));
		m_cameraUp = glm::normalize(glm::cross(m_cameraRight, m_cameraFront));
	}

	void CameraSystem::Receive(Event* ev)
	{}

	void CameraSystem::Shutdown()
	{}
}