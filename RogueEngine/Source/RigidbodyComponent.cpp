#pragma once
#include <sstream>
#include "RigidbodyComponent.h"
#include "REMath.h"

namespace Rogue
{
	RigidbodyComponent::RigidbodyComponent(float mass, float volume, float damping)
		: m_acceleration{ Vec2() },
		m_velocity{ Vec2() },
		m_accForce{ Vec2() },
		m_invMass{ 1 / mass },
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

	std::string RigidbodyComponent::Serialize()
	{
		//Acceleration, Velocity, Mass, Volume, isStatic
		std::ostringstream ss;
		ss << m_acceleration.x << ";" << m_acceleration.y << ";";
		ss << m_velocity.x << ";" << m_velocity.y << ";";
		ss << m_invMass << ";";
		ss << m_volume << ";";
		ss << m_isStatic;

		return ss.str();
	}

	void RigidbodyComponent::Deserialize(std::string toDeserialize)
	{
		std::istringstream ss(toDeserialize);
		std::string s1, s2;		//s2 is used if two are needed
		int counter = 0;		//Needed to take in for multiple values
		int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1 parameter to set

		while (std::getline(ss, s1, ';'))
		{
			//If counter has not cleared all sets yet, read into s2
			if (counter < sets)
				std::getline(ss, s2, ';');

			switch (counter)
			{
			case 0:
				setAcceleration(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 1:
				setVelocity(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 2:
				setMass(std::stof(s1));
				break;
			case 3:
				setVolume(std::stof(s1));
				break;
			case 4:
				setIsStatic(std::stoi(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}

}
