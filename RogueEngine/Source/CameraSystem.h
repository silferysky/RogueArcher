#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "Vector2D.h"
#include "CameraShake.h"

#include <glm.hpp>

namespace Rogue
{
	class Timer;

	class CameraSystem : public System, public EventListener
	{
		glm::vec3 m_cameraPos;

		glm::vec3 m_cameraFront;
		glm::vec3 m_cameraUp;
		glm::vec3 m_cameraRight;
		glm::vec3 m_worldUp;

		float m_cameraVelocity = 1.0f;

		CameraShake m_cameraShake;
	public:
		CameraSystem();
		~CameraSystem() = default;

		glm::mat4 GetViewMatrix();

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}