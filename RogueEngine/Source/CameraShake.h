/* Start Header ************************************************************************/
/*!
\file           CameraShake.h
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
		void Update();

		void SetShake(float magnitude);
		float GetShake() const;

		Vec2 getOffset() const;
	};
}