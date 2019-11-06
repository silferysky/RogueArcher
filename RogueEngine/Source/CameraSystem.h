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
		glm::vec3 m_cameraPos;

		glm::vec3 m_cameraFront;
		glm::vec3 m_cameraUp;
		glm::vec3 m_cameraRight;
		glm::vec3 m_worldUp;

		float m_cameraVelocity = 1.0f;

		Vec2 m_cameraMin;
		Vec2 m_cameraMax;

		Vec2 m_target;

		CameraShake m_cameraShake;

		bool m_worldCamera;
	public:
		CameraSystem();
		~CameraSystem() = default;

		glm::mat4 GetViewMatrix();
		void ToggleWorldCamera();
		void ResetCamera();

		CameraShake GetShake() const;

		Vec2 GetCameraMin() const;
		Vec2 GetCameraMax() const;
		bool GetWorldCamera() const;
		void SetWorldCamera(const bool& camera);

		glm::vec3 GetCameraPos() const;

		void SetCameraMin(const Vec2& cameraMin);
		void SetCameraMax(const Vec2& cameraMax);

		void SetCameraPos(const glm::vec3& position);

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event* ev) override;
	};
}