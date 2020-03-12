/*Start Header*************************************************************************/
/*!
\file           CircleCollider2DComponent.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CircleCollider2DComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CircleCollider2DComponent.h"


namespace Rogue
{
	CircleCollider2DComponent::CircleCollider2DComponent(CircleCollider2DComponent&& rhs) noexcept :
		m_collider{}, m_isCollided{ false }
	{
		std::swap(m_collider, rhs.m_collider);
	}

	CircleCollider2DComponent& CircleCollider2DComponent::operator=(const CircleCollider2DComponent& rhs)
	{
		if (this != &rhs)
		{
			m_collider = rhs.m_collider;
		}
		return *this;
	}

	CircleCollider2DComponent& CircleCollider2DComponent::operator=(CircleCollider2DComponent&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(m_collider, rhs.m_collider);
		}
		return *this;
	}

	CollisionMode CircleCollider2DComponent::GetCollisionMode() const
	{
		return m_collisionMode;
	}

	void CircleCollider2DComponent::SetCollisionMode(CollisionMode mode)
	{
		m_collisionMode = mode;
	}
	
	bool CircleCollider2DComponent::GetIsCollided() const
	{
		return m_isCollided;
	}

	void CircleCollider2DComponent::SetIsCollided(bool isCollided)
	{
		m_isCollided = isCollided;
	}

	void CircleCollider2DComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		float radius = m_collider.getRadius();
		Vec2 centerOffset = m_collider.getCenterOffSet();

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Radius", &radius, 0.5f, -100000.0f, 100000.0f);
		m_collider.setRadius(radius);

		ImGui::DragFloat("Center Offset X ", &centerOffset.x, 0.5f, -100000.0f, 100000.0f);
		ImGui::DragFloat("Center Offset Y ", &centerOffset.y, 0.5f, -100000.0f, 100000.0f);
		m_collider.setCenterOffSet(centerOffset);
		
		ImGui::PushItemWidth(75);

		bool oldAsleep = (m_collisionMode == CollisionMode::e_asleep);
		bool oldAwake = (m_collisionMode == CollisionMode::e_awake);
		bool oldTrigger = (m_collisionMode == CollisionMode::e_trigger);

		bool newAsleep = oldAsleep;
		bool newAwake = oldAwake;
		bool newTrigger = oldTrigger;

		ImGui::Checkbox("Asleep", &newAsleep);
		ImGui::Checkbox("Awake", &newAwake);
		ImGui::Checkbox("Trigger", &newTrigger);

		if (oldAsleep)
		{
			if (newAwake)
				SetCollisionMode(CollisionMode::e_awake);
			else if (newTrigger)
				SetCollisionMode(CollisionMode::e_trigger);
		}
		else if (oldAwake)
		{
			if (newTrigger)
				SetCollisionMode(CollisionMode::e_trigger);
			else if (newAsleep)
				SetCollisionMode(CollisionMode::e_asleep);
		}
		else if (oldTrigger)
		{
			if (newAwake)
				SetCollisionMode(CollisionMode::e_awake);
			else if (newAsleep)
				SetCollisionMode(CollisionMode::e_asleep);
		}
	}

	std::string CircleCollider2DComponent::Serialize()
	{
		//Radius
		std::ostringstream ss;
		ss << m_collider.getRadius() << ";";

		ss << m_collider.getCenterOffSet().x << ";";
		ss << m_collider.getCenterOffSet().y << ";";
		ss << m_collider.getScaleOffSet().x << ";";
		ss << m_collider.getScaleOffSet().y;

		switch (m_collisionMode)
		{
		case CollisionMode::e_awake:
			ss << "AWAKE";
			break;

		case CollisionMode::e_trigger:
			ss << "TRIGGER";
			break;

		case CollisionMode::e_asleep:
			ss << "ASLEEP";
			break;
		}
		return ss.str();
	}

	void CircleCollider2DComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1;			//s2 is used if two are needed
		//int counter = 0;		//Needed to take in for multiple values
		//int sets = 2;			//Sets represents the number of "sets" that must be taken in simultaneously. Aka vec2 or more than 1	parameter to set

		if (std::getline(ss, s1, ';'))
			m_collider.setRadius(std::stof(s1));

		// For Offsets
		float x = 0.0f;
		float y = 0.0f;

		if (std::getline(ss, s1, ';'))
			x = stof(s1);

		if (std::getline(ss, s1, ';'))
			y = stof(s1);

		m_collider.setCenterOffSet(Vec2{ x, y });

		if (std::getline(ss, s1, ';'))
			x = stof(s1);

		if (std::getline(ss, s1, ';'))
			y = stof(s1);

		m_collider.setScaleOffSet(Vec2{ x, y });

		if (std::getline(ss, s1, ';'))
		{
			if (s1 == "AWAKE")
				m_collisionMode = CollisionMode::e_awake;
			else if (s1 == "TRIGGER")
				m_collisionMode = CollisionMode::e_trigger;
			else if (s1 == "ASLEEP")
				m_collisionMode = CollisionMode::e_asleep;
		}
	}
}