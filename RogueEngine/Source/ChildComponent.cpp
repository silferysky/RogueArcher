#pragma once
#include "Precompiled.h"
#include "ChildComponent.h"

namespace Rogue
{
	ChildComponent::ChildComponent()
		: m_globalDirty{ false }, m_localDirty{ false }, m_isFollowing{ true }, m_parent{ MAX_ENTITIES }, m_position{ Vec2() }, m_scale{ Vec2() }, m_positionZ{ 0 }, m_rotation{ 0 }
	{}

	void ChildComponent::SetLocalDirty()
	{
		m_localDirty = true;
	}

	void ChildComponent::SetGlobalDirty()
	{
		m_globalDirty = true;
	}

	void ChildComponent::ResetLocalDirty()
	{
		m_localDirty = false;
	}

	void ChildComponent::ResetGlobalDirty()
	{
		m_globalDirty = false;
	}

	bool ChildComponent::IsLocalDirty() const
	{
		return m_localDirty;
	}

	bool ChildComponent::IsGlobalDirty() const
	{
		return m_globalDirty;
	}

	void ChildComponent::SetIsFollowing(bool follow)
	{
		m_isFollowing = follow;
	}

	bool ChildComponent::IsFollowing() const
	{
		return m_isFollowing;
	}

	void ChildComponent::SetParent(Entity ent)
	{
		m_parent = ent;
	}

	void ChildComponent::ResetParent()
	{
		m_parent = -1;
	}

	Entity ChildComponent::GetParent() const
	{
		return m_parent;
	}

	void ChildComponent::SetPosition(Vec2 position)
	{
		m_position = position;
	}

	void ChildComponent::SetPosition(float x, float y)
	{
		m_position = Vec2(x, y);
	}

	Vec2 ChildComponent::GetPosition() const
	{
		return m_position;
	}

	void ChildComponent::SetPositionZ(int z)
	{
		m_positionZ = z;
	}

	int ChildComponent::GetPositionZ() const
	{
		return m_positionZ;
	}

	void ChildComponent::SetScale(Vec2 scale)
	{
		m_scale = scale;
	}

	void ChildComponent::SetScale(float x, float y)
	{
		m_scale = Vec2(x, y);
	}

	Vec2 ChildComponent::GetScale() const
	{
		return m_scale;
	}

	void ChildComponent::SetRotation(float rotate)
	{
		m_rotation = rotate;
	}

	float ChildComponent::GetRotation() const
	{
		return m_rotation;
	}

	std::string ChildComponent::Serialize()
	{
		return std::string();
		//std::ostringstream oss;
		//oss << m_isFollowing << ";";
		//return oss.str();
	}

	void ChildComponent::Deserialize(std::string_view toDeserialize)
	{
		//std::istringstream ss(toDeserialize.data());
		//std::string s1;

		//if (std::getline(ss, s1, ';'))
		//	m_isFollowing = std::stoi(s1);
	}

	void ChildComponent::DisplayOnInspector()
	{
		ImGui::Checkbox("Component Active", &m_componentIsActive);
		bool isDirtyCheck = false;

		//If this function is updated, update EditorInspector's PositionDisplay also
		ImGui::Text("Translate");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("##TranslateX", &m_position.x);

		if (ImGui::IsItemDeactivated())
			isDirtyCheck = true;

		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("##TranslateY", &m_position.y);

		if (ImGui::IsItemDeactivated())
			isDirtyCheck = true;

		ImGui::Text("Z Value  ");
		ImGui::SameLine();
		ImGui::DragInt("##TranslateZ", &m_positionZ, 1.0f, -100000, 100000);

		if (ImGui::IsItemDeactivated())
			isDirtyCheck = true;

		if (ImGui::IsItemHovered())
		{
			ImGui::BeginTooltip();
			ImGui::Text("Higher number means the object will be drawn infront");
			ImGui::EndTooltip();
		}

		ImGui::Text("Scale    ");
		ImGui::SameLine();
		ImGui::PushItemWidth(75);
		ImGui::DragFloat("##ScaleX", &m_scale.x, 1.0f, 0.0f, 100000.0f);

		if (ImGui::IsItemDeactivated())
			isDirtyCheck = true;

		ImGui::SameLine(0.0f, 36.0f);
		ImGui::DragFloat("##ScaleY", &m_scale.y, 1.0f, 0.0f, 100000.0f);

		if (ImGui::IsItemDeactivated())
			isDirtyCheck = true;

		ImGui::Text("Rotation ");
		ImGui::SameLine();
		ImGui::DragFloat("##Rotation", &m_rotation, 0.1f, 0.0f, 6.28f);

		if (ImGui::IsItemDeactivated())
			isDirtyCheck = true;

		if (ImGui::Button("Reset Position"))
		{
			m_position = Vec2{ 0.0f, 0.0f };
			isDirtyCheck = true;
		}

		if (ImGui::Button("Reset Z"))
		{
			m_positionZ = 0;
			isDirtyCheck = true;
		}

		if (ImGui::Button("Reset Rotation"))
		{
			m_rotation = 0.0f;
			isDirtyCheck = true;
		}

		if (ImGui::Button("Reset Scale"))
		{
			m_scale = Vec2{ 1.0f, 1.0f };
			isDirtyCheck = true;
		}

		if (isDirtyCheck)
		{
			SetGlobalDirty();
		}
	}
}