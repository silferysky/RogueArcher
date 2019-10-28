#pragma once

namespace Rogue
{
	struct MassData
	{
		float m_mass;
		float m_inertia;

		float m_invMass;
		float m_invInertia;

		MassData(float mass = 1.0f, float inertia = 1.0f) :
			m_mass{ mass }, m_inertia{ inertia },
			m_invMass{ mass ? 1.0f / mass : 0.0f }, m_invInertia{ inertia ? 1.0f / mass : 0.0f }
		{}
	};




} // ns Rogue