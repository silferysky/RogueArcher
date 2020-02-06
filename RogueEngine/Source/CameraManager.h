/* Start Header ************************************************************************/
/*!
\file           CameraManager.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CameraManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Vector2D.h"
#define CAMERA_MANAGER CameraManager::instance()

namespace Rogue
{
	class CameraManager
	{	
		Vec2 m_cameraMin;
		Vec2 m_cameraMax;

		glm::vec3 m_cameraPos;
		float m_cameraZoom;
		float m_levelCameraZoom;
	public:
		// Singleton instance
		static CameraManager& instance()
		{
			static CameraManager instance;
			return instance;
		}

		// Default constructor
		CameraManager() :
			m_cameraMin{ -1600.0f, -1600.0f },
			m_cameraMax{ 1600.0f, 1600.0f },
			m_cameraPos{ 0.0f, 0.0f, 0.0f },
			m_cameraZoom{ 1.0f },
			m_levelCameraZoom{ 1.0f }
		{}

		Vec2 GetCameraMin() const { return m_cameraMin; }
		Vec2 GetCameraMax() const { return m_cameraMax; }
		glm::vec3 GetCameraPos() const { return m_cameraPos; }

		void SetCameraMin(const Vec2& min) { m_cameraMin = min; }
		void SetCameraMax(const Vec2& max) { m_cameraMax = max; }
		void SetCameraPos(const glm::vec3& position) { m_cameraPos = position; }

		void ZoomIn()
		{
			if (m_cameraZoom > 0.0f)
				m_cameraZoom -= 0.01f;
		}

		void ZoomOut()
		{
			m_cameraZoom += 0.01f; 
		}

		float GetCameraZoom()
		{
			return m_cameraZoom;
		}

		void SetCameraZoom(float cameraZoom)
		{
			m_cameraZoom = cameraZoom;
		}

		float GetLevelCameraZoom()
		{
			return m_levelCameraZoom;
		}

		void SetLevelCameraZoom(float cameraZoom)
		{
			m_levelCameraZoom = cameraZoom;
		}
	};
}