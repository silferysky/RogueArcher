#pragma once
#include "Vector2D.h"

namespace Rogue
{
	class CameraManager
	{	
		// Min/Max
		Vec2 m_cameraMin;
		Vec2 m_cameraMax;
	public:
		static CameraManager& instance()
		{
			static CameraManager instance;
			return instance;
		}

		CameraManager() :
			m_cameraMin{ -1600.0f, -1600.0f }, m_cameraMax{ 1600.0f, 1600.0f }
		{}

		Vec2 GetCameraMin() const { return m_cameraMin; }
		Vec2 GetCameraMax() const { return m_cameraMax; }

		void SetCameraMin(const Vec2& min) { m_cameraMin = min; }
		void SetCameraMax(const Vec2& max) { m_cameraMax = max; }
	};
}