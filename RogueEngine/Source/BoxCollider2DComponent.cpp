/*Start Header*************************************************************************/
/*!
\file           BoxCollider2DComponent.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BoxCollider2DComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "BoxCollider2DComponent.h"
#include "Vector2D.h"
#include "Logger.h"

namespace Rogue
{
	BoxCollider2DComponent::BoxCollider2DComponent() :
		m_aabb{}, m_collisionMode{ CollisionMode::e_awake }, m_isCollided{ false }
	{}

	BoxCollider2DComponent::BoxCollider2DComponent(const BoxCollider2DComponent& rhs) :
		m_aabb{ rhs.m_aabb }, m_collisionMode{ rhs.m_collisionMode }, m_isCollided{ false }
	{}

	BoxCollider2DComponent::BoxCollider2DComponent(BoxCollider2DComponent&& rhs) noexcept :
		m_aabb{ AABB{} }, m_collisionMode{}, m_isCollided{ false }
	{
		std::swap(m_aabb, rhs.m_aabb);
		std::swap(m_collisionMode, rhs.m_collisionMode);
	}

	BoxCollider2DComponent& BoxCollider2DComponent::operator=(const BoxCollider2DComponent& rhs)
	{
		if (this != &rhs)
		{
			m_aabb.operator=(rhs.m_aabb);
			m_collisionMode = rhs.m_collisionMode;
		}
		return *this;
	}

	BoxCollider2DComponent& BoxCollider2DComponent::operator=(BoxCollider2DComponent&& rhs) noexcept
	{
		if (this != &rhs)
		{
			std::swap(m_aabb, rhs.m_aabb);
			std::swap(m_collisionMode, rhs.m_collisionMode);
		}

		return *this;
	}

	CollisionMode BoxCollider2DComponent::GetCollisionMode() const
	{
		return m_collisionMode;
	}

	void BoxCollider2DComponent::SetCollisionMode(CollisionMode mode)
	{
		m_collisionMode = mode;
	}

	bool BoxCollider2DComponent::GetIsCollided() const
	{
		return m_isCollided;
	}

	void BoxCollider2DComponent::SetIsCollided(bool isCollided)
	{
		m_isCollided = isCollided;
	}

	std::string BoxCollider2DComponent::Serialize()
	{
		std::ostringstream ss;

#if 0
		//Size, modelVertexList
		ss << m_obb.getSize() << ";";

		for (size_t i = 0; i < m_obb.getSize(); ++i)
		{
			ss << m_obb.modelVerts()[i].x << ";" << m_obb.modelVerts()[i].y << ";";
		}
#endif
		ss << m_aabb.getCenterOffSet().x << ";";
		ss << m_aabb.getCenterOffSet().y << ";";

		ss << m_aabb.getScaleOffSet().x << ";";
		ss << m_aabb.getScaleOffSet().y << ";";

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

	void BoxCollider2DComponent::Deserialize(std::string_view toDeserialize)
	{
		std::istringstream ss(toDeserialize.data());
		std::string s1, s2;		//s2 is used if two are needed
		std::vector<Vec2> vertexList{};
		size_t size = 0;

		// Shouldn't be used anymore.
#if 0
		// For OBB
		if (std::getline(ss, s1, ';'))
		{
			size = static_cast<size_t>(stoi(s1));
		}

		for (size_t counter = 0; counter < size; counter++)
		{
			std::getline(ss, s1, ';');
			std::getline(ss, s2, ';');
			vertexList.emplace_back(Vec2(stof(s1), stof(s2)));
		}
#endif

		// For AABB
		float x = 0.0f;
		float y = 0.0f;

		if(std::getline(ss, s1, ';'))
			x = std::stof(s1);

		if (std::getline(ss, s1, ';'))
			y = std::stof(s1);

		m_aabb.setCenterOffSet(Vec2{ x, y });

		if (std::getline(ss, s1, ';'))
			x = std::stof(s1);

		if (std::getline(ss, s1, ';'))
			y = std::stof(s1);

		m_aabb.setScaleOffSet(Vec2{ x, y });

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

	void BoxCollider2DComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		Vec2 m_scale = m_aabb.getScaleOffSet();
		Vec2 m_center = m_aabb.getCenterOffSet();

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Scale X", &m_scale.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Scale Y", &m_scale.y, 1.0f, -10000.0f, 10000.0f);
		m_aabb.setScaleOffSet(m_scale);

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Center X", &m_center.x, 1.0f, -10000.0f, 10000.0f);
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Center Y", &m_center.y, 1.0f, -10000.0f, 10000.0f);
		m_aabb.setCenterOffSet(m_center);

		ImGui::PushItemWidth(75);

		bool oldAsleep =  (m_collisionMode == CollisionMode::e_asleep);
		bool oldAwake = (m_collisionMode == CollisionMode::e_awake);
		bool oldTrigger = (m_collisionMode == CollisionMode::e_trigger);

		bool newAsleep = oldAsleep;
		bool newAwake = oldAwake;
		bool newTrigger = oldTrigger;

		ImGui::Checkbox("Asleep",&newAsleep);
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
}