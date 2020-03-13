/* Start Header ************************************************************************/
/*!
\file           RigidBodyComponent.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for RigidBodyComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Precompiled.h"
#include "RigidbodyComponent.h"
#include "REMath.h"
#include "Logger.h"

namespace Rogue
{

	RigidbodyComponent::RigidbodyComponent()
		: m_acceleration{ Vec2() },
		m_velocity{ Vec2() },
		m_accForce{ Vec2() },
		m_volume{ 1.0f },
		m_damping{ 0.50f },
		m_isStatic{ false },
		m_restitution{ 0.1f },
		m_friction{ 0.01f },
		m_gravityScale{ 1.0f },
		m_massData{}
	{}

	Vec2 RigidbodyComponent::getVelocity() const
	{
		return m_velocity;
	}

	float RigidbodyComponent::getInvMass() const
	{
		return m_massData.m_invMass;
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

	float RigidbodyComponent::getMass() const
	{
		return m_massData.m_mass;
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
		RE_ASSERT(mass > RE_EPSILON, "Mass is 0! Check serialization.");

		m_massData.m_mass = mass;
		m_massData.m_invMass = 1 / mass;
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

	float RigidbodyComponent::getBounciness() const
	{
		return m_restitution;
	}

	float RigidbodyComponent::getFriction() const
	{
		return m_friction;
	}

	float RigidbodyComponent::getGravityScale() const
	{
		return m_gravityScale;
	}

	void RigidbodyComponent::setDamping(float damping)
	{
		m_damping = damping;
	}

	void RigidbodyComponent::setIsStatic(bool set)
	{
		m_isStatic = set;

		if (set == true)
			m_massData.m_invMass = 0.0f;
		else
			m_massData.m_invMass = 1.0f;
}

	void RigidbodyComponent::setBounciness(float bounce)
	{
		m_restitution = bounce;
	}

	void RigidbodyComponent::setFriction(float friction)
	{
		m_friction = friction;
	}

	void RigidbodyComponent::setGravityScale(float gravity)
	{
		m_gravityScale = gravity;
	}

	void RigidbodyComponent::DisplayOnInspector()
	{
		// Note: Changed to use a temp variable for a debug attempt

		ImGui::Checkbox("Component Active", &m_componentIsActive);

		ImGui::PushItemWidth(75);
		ImGui::Checkbox("Static?", &m_isStatic);

		if (m_isStatic)
			m_massData.m_invMass = 0.0f;
		else
			m_massData.m_invMass = 1.0f;

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Velocity X", &m_velocity.x, 1.0f, -2000.0f, 2000.0f);
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Velocity Y", &m_velocity.y, 1.0f, -2000.0f, 2000.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Acceleration X", &m_acceleration.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Acceleration Y", &m_acceleration.y, 1.0f, -10000.0f, 10000.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Friction", &m_friction, 1.0f, 0.0f, 1.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Damping", &m_damping, 1.0f, 0.0f, 1.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Bounciness", &m_restitution, 1.0f, 0.0f, 1.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Gravity Scale", &m_gravityScale, 1.0f, 0.0f, 100.0f);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Mass", &m_massData.m_mass, 0.05f, 0.01f, 10000.0f);

		m_massData.m_invMass = 1 / m_massData.m_mass;
	}

	std::string RigidbodyComponent::Serialize()
	{
		//Acceleration, Velocity, Mass, Volume, isStatic
		std::ostringstream ss;
		ss
			<< m_acceleration.x << ";" << m_acceleration.y << ";";
		ss
			<< m_velocity.x << ";" << m_velocity.y << ";";
		if (m_massData.m_invMass != 0.0f)
			ss
				<< m_massData.m_invMass << ";";
		else
			ss
				<< 1 << ";";
		ss
			<< m_volume << ";"
			<< m_isStatic << ";"
			<< m_gravityScale << ";"
			<< m_damping << ";"
			<< m_restitution << ";"
			<< m_friction << ";";

		return ss.str();
	}

	void RigidbodyComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
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
				m_acceleration = Vec2(std::stof(s1), std::stof(s2));
				break;
			case 1:
				m_velocity = Vec2(std::stof(s1), std::stof(s2));
				break;
			case 2:
				setMass(std::stof(s1)); // Always call set mass to set correctly.
				break;
			case 3:
				m_volume = std::stof(s1);
				break;
			case 4:
				setIsStatic(std::stoi(s1));
				break;
			case 5:
				m_gravityScale = std::stof(s1);
				break;
			case 6:
				m_damping = std::stof(s1);
				break;
			case 7:
				m_restitution = std::stof(s1);
				break;
			case 8:
				m_friction = std::stof(s1);
				break;
			default:
				break;
			}

			++counter;
		}
	}
}