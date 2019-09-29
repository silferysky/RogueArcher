#pragma once
#include "Rigidbody.h"


Rigidbody::Rigidbody(float mass, float volume, float damping)
	: m_acceleration{ Vec2() },
	  m_velocity{ Vec2() },
	  m_accForce{ Vec2() },
	  m_invMass{ 1/mass },
	  m_volume{ volume },
	  m_damping{ damping }
{}

Vec2 Rigidbody::getVelocity() const
{
	return m_velocity;
}

float Rigidbody::getInvMass() const
{
	return m_invMass;
}

float Rigidbody::getVolume() const
{
	return m_volume;
}

Vec2 Rigidbody::getAccForce() const
{
	return m_accForce;
}

Vec2 Rigidbody::getAcceleration() const
{
	return m_acceleration;
}

void Rigidbody::setAccForce(const Vec2& accforce)
{
	m_accForce = accforce;
}

void Rigidbody::setVelocity(const Vec2& vel)
{
	m_velocity = vel;
}

void Rigidbody::offSetVelocity(const Vec2& vel)
{
	m_velocity += vel;
}

void Rigidbody::setMass(float mass)
{
	if (mass < RE_EPSILON && mass > -RE_EPSILON)
		throw("Mass is 0!");

	m_invMass = 1 / mass;
}

void Rigidbody::setVolume(float volume)
{
	m_volume = volume;
}

void Rigidbody::addForce(const Vec2& force)
{
	m_accForce += force;
}

void Rigidbody::setAcceleration(const Vec2& accel)
{
	m_acceleration = accel;
}

void Rigidbody::offSetAcceleration(const Vec2& accel)
{
	m_acceleration += accel;
}

float Rigidbody::getDamping() const
{
	return m_damping;
}

void Rigidbody::setDamping(float damping)
{
	m_damping = damping;
}