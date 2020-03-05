/* Start Header ************************************************************************/
/*!
\file           CameraSystem.h
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
		Vec2 m_centerPosition;
		Vec2 m_target;
		float m_cameraLerp = 1.6f;

		CameraShake m_cameraShake;
		std::vector<Entity>& m_currentVector;
		bool m_worldCamera;
		bool m_isActive = true;
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

		glm::vec3 GetCameraFront() const;
		glm::vec3 GetCameraUp() const;

		void setIsActive(const bool& isActive);
		bool getIsActive() const;

		void Init() override;
		void Update() override;
		void Shutdown() override;
		void Receive(Event& ev) override;
	};
}