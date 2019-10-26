#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "Vector2D.h"

#include <glm.hpp>

namespace Rogue
{
	class Timer;

	class CameraSystem : public System, public EventListener
	{
		glm::vec3 cameraPos;
		glm::vec3 cameraTarget;
		glm::vec3 cameraDirection;

		glm::vec3 cameraRight;

		glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

		glm::mat4 view;
	public:
		CameraSystem();
		~CameraSystem() = default;

		void init();
		void update() override;
		void receive(Event* ev) override;
	};
}