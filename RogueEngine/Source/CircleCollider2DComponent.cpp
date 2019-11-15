#include "Precompiled.h"
#include "CircleCollider2DComponent.h"


namespace Rogue
{
	CircleCollider2DComponent::CircleCollider2DComponent(CircleCollider2DComponent&& rhs) noexcept :
		m_collider{ CircleCollider{} }
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
	void CircleCollider2DComponent::DisplayOnInspector()
	{
		float m_radius = m_collider.getRadius();
		Vec2 m_centerOffset = m_collider.getCenterOffSet();
		float m_rotationOffset = m_collider.getRotationOffSet();
		Vec2 m_scaleOffset = m_collider.getScaleOffSet();

		ImGui::PushItemWidth(75);
		ImGui::DragFloat("Radius", &m_radius, 0.5f, -100000.0f, 100000.0f);
		m_collider.setRadius(m_radius);

		ImGui::DragFloat("Center Offset X ", &m_centerOffset.x, 0.5f, -100000.0f, 100000.0f);
		ImGui::DragFloat("Center Offset Y ", &m_centerOffset.y, 0.5f, -100000.0f, 100000.0f);
		m_collider.setCenterOffSet(m_centerOffset);

		ImGui::DragFloat("Rotation Offset ", &m_rotationOffset, 0.5f, -100000.0f, 100000.0f);
		m_collider.setRotationOffSet(m_rotationOffset);

		ImGui::DragFloat("Scale Offset X ", &m_scaleOffset.x, 0.5f, -100000.0f, 100000.0f);
		ImGui::DragFloat("Scale Offset Y ", &m_scaleOffset.y, 0.5f, -100000.0f, 100000.0f);
		m_collider.setScaleOffSet(m_scaleOffset);
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
	}
}