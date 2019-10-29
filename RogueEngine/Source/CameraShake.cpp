#include "CameraShake.h"

namespace Rogue
{
	CameraShake::CameraShake() : m_shakeAngleMagnitude{ 180 }, m_shakeInterval{ 0.05f }, m_currentShakeMagnitude{ 0.0f }, m_currentShakeAngle{ rand() % 360 }, m_duration{ 0 }, m_shakeOffset{ 0, 0 }
	{}

	void CameraShake::Update(float dt)
	{
		m_duration += dt;

		//Updates the angle and shake magnitude when time every ShakeInterval seconds
		if (m_duration > m_shakeInterval && m_currentShakeMagnitude > 0) 
		{
			m_duration = 0;

			m_currentShakeMagnitude *= 0.9f; //diminish radius each frame
			m_currentShakeAngle += (180 + (-(m_shakeAngleMagnitude / 2) + /* int(m_pn.noise(0.45, 0.8, 0.55)) */ rand() % m_shakeAngleMagnitude)); //pick new angle 

			m_shakeOffset.x = static_cast<float>(sin(m_currentShakeAngle) * m_currentShakeMagnitude);
			m_shakeOffset.y = static_cast<float>(cos(m_currentShakeAngle) * m_currentShakeMagnitude);

			//Don't micro shake the screen, also returns the screen back to centre after shaking is done
			if (m_currentShakeMagnitude < 0.2f) 
			{
				m_currentShakeMagnitude = 0;
				m_shakeOffset = { 0,0 };
			}
		}
	}

	void CameraShake::SetShake(float magnitude)
	{
		m_currentShakeMagnitude = magnitude;
	}

	Vec2 CameraShake::getOffset() const
	{
		return m_shakeOffset;
	}
}