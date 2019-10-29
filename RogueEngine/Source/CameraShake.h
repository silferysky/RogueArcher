#pragma once
#include "Vector2D.h"
//#include "PerlinNoise.h"

namespace Rogue
{
	class CameraShake
	{
		int m_shakeAngleMagnitude;
		int m_currentShakeAngle;

		float m_shakeInterval; //If dt > m_i_ShakeInterval, then update screenshake magnitude
		float m_currentShakeMagnitude;
		float m_duration;

		//PerlinNoise m_pn;

		Vec2 m_shakeOffset;
	public:
		CameraShake();
		void Update(float dt);

		void SetShake(float magnitude);

		Vec2 getOffset() const;
	};
}