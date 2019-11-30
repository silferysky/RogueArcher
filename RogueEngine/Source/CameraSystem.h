#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include <glm.hpp>
#include "CameraShake.h"

namespace Rogue
{
	class Timer;

	class CameraSystem : public System, public EventListener
	{
		glm::vec3 m_cameraFront;
		glm::vec3 m_cameraUp;
		glm::vec3 m_cameraRight;
		glm::vec3 m_worldUp;

		// Camera panning
		Vec2 m_target;
		float m_cameraLerp = 2.0f;

		CameraShake m_cameraShake;

		bool m_worldCamera;
	public:
		CameraSystem();
		~CameraSystem() = default;

		glm::mat4 GetViewMatrix(const float& parallax = 1.0f);
		void ToggleWorldCamera();
		void ResetCamera();

		CameraShake GetShake() const;
		float GetLerp() const;

		bool GetWorldCamera() const;
		void SetWorldCamera(const bool& camera);

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}