#pragma once
#include "RigidbodyComponent.h"


RigidbodyComponent::RigidbodyComponent(float mass, float volume, float damping)
	: m_acceleration{ Vec2() },
	  m_velocity{ Vec2() },
	  m_accForce{ Vec2() },
	  m_invMass{ 1/mass },
	  m_volume{ volume },
	  m_damping{ damping },
	  m_isStatic{ false }
{}

Vec2 RigidbodyComponent::getVelocity() const
{
	return m_velocity;
}

float RigidbodyComponent::getInvMass() const
{
	return m_invMass;
}

float RigidbodyComponent::getVolume() const
{
	return m_volume;
}

Vec2 RigidbodyComponent::getAccForce() const
{
	return m_accForce;
}

Vec2 RigidbodyComponent::getAcceleration() const
{
	return m_acceleration;
}

void RigidbodyComponent::setAccForce(const Vec2& accforce)
{
	m_accForce = accforce;
}

void RigidbodyComponent::setVelocity(const Vec2& vel)
{
	m_velocity = vel;
}

void RigidbodyComponent::offSetVelocity(const Vec2& vel)
{
	m_velocity += vel;
}

void RigidbodyComponent::setMass(float mass)
{
	if (mass < REMath::EPSILON && mass > -REMath::EPSILON)
		throw("Mass is 0!");

	m_invMass = 1 / mass;
}

void RigidbodyComponent::setVolume(float volume)
{
	m_volume = volume;
}

void RigidbodyComponent::addForce(const Vec2& force)
{
	m_accForce += force;
}

void RigidbodyComponent::setAcceleration(const Vec2& accel)
{
	m_acceleration = accel;
}

void RigidbodyComponent::offSetAcceleration(const Vec2& accel)
{
	m_acceleration += accel;
}

float RigidbodyComponent::getDamping() const
{
	return m_damping;
}

bool RigidbodyComponent::getIsStatic() const
{
	return m_isStatic;
}

void RigidbodyComponent::setDamping(float damping)
{
	m_damping = damping;
}

void RigidbodyComponent::setIsStatic(bool set)
{
	m_isStatic = set;
}
