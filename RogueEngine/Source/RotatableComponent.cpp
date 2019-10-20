#include "RotatableComponent.h"

inline void RotatableComponent::computeMOI(float mass, float volume)
{
	m_moInertia = mass * volume / 12;
}