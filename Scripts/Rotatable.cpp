#include "Physics.h"


inline void Rotatable::computeMOI(float mass, float volume)
{
	m_moInertia = mass * volume / 12;
}