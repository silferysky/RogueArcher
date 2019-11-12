#include "Precompiled.h"
#include "RotatableComponent.h"

namespace Rogue
{
	inline void RotatableComponent::computeMOI(float mass, float volume)
	{
		m_moInertia = mass * volume / 12;
	}
}