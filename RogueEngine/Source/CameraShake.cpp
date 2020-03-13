/* Start Header ************************************************************************/
/*!
\file           CameraShake.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CameraShake

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CameraShake.h"
#include "Main.h"
#define PI 3.141592

namespace Rogue
{
	CameraShake::CameraShake() : m_shakeAngleMagnitude{ 180 }, m_shakeInterval{ 0.05f }, m_currentShakeMagnitude{ 0.0f }, m_currentShakeAngle{ rand() % 360 }, m_duration{ 0 }, m_shakeOffset{ 0, 0 }
	{}

	void CameraShake::Update()
	{
		m_duration += g_fixedDeltaTime; //* g_engine.GetTimeScale();

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
				g_engine.m_coordinator.GetSystem<CameraSystem>()->setIsActive(true);
			}
		}
	}

	void CameraShake::SetShake(float magnitude)
	{
		m_currentShakeMagnitude = magnitude;
	}

	float CameraShake::GetShake() const
	{
		return m_currentShakeMagnitude;
	}

	Vec2 CameraShake::getOffset() const
	{
		return m_shakeOffset;
	}
}