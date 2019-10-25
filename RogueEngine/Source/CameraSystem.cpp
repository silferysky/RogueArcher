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

	void CameraSystem::init()
	{
		REGISTER_LISTENER(SystemID::id_CAMERASYSTEM, CameraSystem::receive);

		// Add components to signature.
		Signature signature;
		// Set system signature.
		gEngine.m_coordinator.SetSystemSignature<CameraSystem>(signature);

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);

		glm::vec3 cameraRight = glm::normalize(glm::cross(glm::vec3(0.0f, 1.0f, 0.0f) , cameraDirection));

		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void CameraSystem::update()
	{
		gEngine.m_coordinator.InitTimeSystem("Camera System");
		

		gEngine.m_coordinator.EndTimeSystem("Camera System");
	}

	void CameraSystem::receive(Event* ev)
	{
	}
}