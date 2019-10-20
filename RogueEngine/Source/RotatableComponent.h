#pragma once

namespace Rogue
{
	class RotatableComponent
	{
		float m_angularVel;
		float m_torque;
		float m_moInertia;

	public:
		inline void computeMOI(float mass, float volume);
	};

}