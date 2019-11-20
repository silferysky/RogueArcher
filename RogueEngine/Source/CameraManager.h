#pragma once
#include "Vector2D.h"

namespace Rogue
{
	class CameraManager
	{	
		Vec2 m_cameraMin;
		Vec2 m_cameraMax;

		glm::vec3 m_cameraPos;
		float m_cameraZoom;
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
			m_cameraZoom{ 1.0f }
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

	};
}