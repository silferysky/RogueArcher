#include "Precompiled.h"
#include "TransformComponent.h"
#include "REMath.h"

namespace Rogue
{
	TransformComponent::TransformComponent(const Vec2& pos, const Vec2& scale, float rot, int Z, const AABB& aabb) :
		m_position{ pos }, m_scale{ scale }, m_rotation{ rot }, m_fakeZ{ Z }, m_pickArea{ aabb }
	{}

	void TransformComponent::setPosition(const Vec2& pos)
	{
		m_position = pos;
	}

	void TransformComponent::offSetPosition(const Vec2& pos)
	{
		m_position += pos;
	}

	void TransformComponent::setScale(const Vec2& scale)
	{
		m_scale = scale;
	}

	void TransformComponent::offSetScale(const Vec2& scale)
	{
		m_scale += scale;
	}

	void TransformComponent::setRotation(float rot)
	{
		m_rotation = rot;
	}

	void TransformComponent::offSetRotation(float rot)
	{
		m_rotation += rot;
	}

	void TransformComponent::setPickArea(const AABB& aabb)
	{
		m_pickArea = aabb;
	}

	void TransformComponent::setZ(int z)
	{
		m_fakeZ = z;
	}

	Vec2 TransformComponent::GetPosition() const
	{
		return m_position;
	}

	Vec2 TransformComponent::GetScale() const
	{
		return m_scale;
	}

	float TransformComponent::GetRotation() const
	{
		return m_rotation;
	}

	const AABB& TransformComponent::GetPickArea() const
	{
		return m_pickArea;
	}

	int TransformComponent::GetZ() const
	{
		return m_fakeZ;
	}

	std::string TransformComponent::Serialize()
	{
		//Position, Scale, Rotation
		std::ostringstream ss;
		ss << m_position.x << ";" << m_position.y << ";";
		ss << m_scale.x << ";" << m_scale.y << ";";
		ss << m_rotation;
		ss << m_fakeZ;
		return ss.str();
	}

	void TransformComponent::Deserialize(std::string_view toDeserialize)
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
				setPosition(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 1:
				setScale(Vec2(std::stof(s1), std::stof(s2)));
				break;
			case 2:
				setRotation(std::stof(s1));
				break;
			case 3:
				setZ(std::stof(s1));
				break;
			default:
				break;
			}

			++counter;
		}
	}

	void TransformComponent::DisplayOnInspector()
	{
		ImGui::Text("Scale    ");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat(" ", &m_scale.x, 1.0f, 0.0f, 100000.0f);
		ImGui::SameLine(0.0f, 36.0f);
		ImGui::DragFloat("  ", &m_scale.y, 1.0f, 0.0f, 100000.0f);


		ImGui::Text("Rotation ");
		ImGui::SameLine();
		ImGui::DragFloat("   ", &m_rotation, 0.1f, 0.0f, 6.28f);
		
		ImGui::Text("Translate");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("     ", &m_position.x);
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("      ", &m_position.y);
		ImGui::PushItemWidth(50);

		static char m_priorityDraw[128];
		ImGui::TextWrapped("Current Z : %d", m_fakeZ);
		ImGui::TextWrapped("Set Z");
		ImGui::InputText("                       ", m_priorityDraw, 128);
		if (ImGui::Button("Set Z"))
		{
			m_fakeZ = atoi(m_priorityDraw);
			setZ(m_fakeZ);
			memset(m_priorityDraw, 0, 128);
		}
		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Higher number means the object will be drawn infront");
			ImGui::EndTooltip();
		}
		
		if (ImGui::Button("Reset Position"))
		{
			m_position = Vec2{ 0.0f, 0.0f };
		}
		
		if (ImGui::Button("Reset Rotation"))
		{
			m_rotation = 0.0f;
		}
		
		if (ImGui::Button("Reset Scale"))
		{
			m_scale = Vec2{ 100.0f,100.0f };
		}
	}
}