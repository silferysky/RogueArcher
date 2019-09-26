#include "Physics.h"

Rigidbody::Rigidbody()
	:BaseComponent(CMP_RIGIDBODY) {}

void Rigidbody::computeForce(const float mass, const Vec2& gravity) // F = ma
{
	m_force = mass * gravity;
}

inline Vec2 Rigidbody::getVelocity() const
{
	return m_velocity;
}

inline float Rigidbody::getMass() const
{
	return m_mass;
}

inline float Rigidbody::getVolume() const
{
	return m_volume;
}

inline float Rigidbody::getAngle() const
{
	return m_orientation;
}

inline Vec2 Rigidbody::getForce() const
{
	return m_force;
}

inline void Rigidbody::setVelocity(const Vec2& vel)
{
	m_velocity = vel;
}

inline void Rigidbody::setMass(float mass)
{
	m_mass = mass;
}


inline void Rigidbody::setVolume(float volume)
{
	m_volume = volume;
}

inline void Rigidbody::setAngle(float angle)
{
	m_orientation = angle;
}

inline void Rigidbody::setForce(const Vec2& force)
{
	m_force = force;
}
