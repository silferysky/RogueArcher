#pragma once
#include "Precompiled.h"
#include "ChildComponent.h"

namespace Rogue
{
	std::string ChildComponent::Serialize()
	{
		return std::string();
	}

	void ChildComponent::Deserialize(std::string_view toDeserialize)
	{
	}

	void ChildComponent::DisplayOnInspector()
	{
	}

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

	void ChildComponent::SetTransform(Vec2 transform)
	{
		m_transform = transform;
	}

	void ChildComponent::SetTransform(float x, float y)
	{
		m_transform = Vec2(x, y);
	}

	Vec2 ChildComponent::GetTransform() const
	{
		return m_transform;
	}

	void ChildComponent::SetTransformZ(float z)
	{
		m_transformZ = z;
	}

	float ChildComponent::GetTransformZ() const
	{
		return m_transformZ;
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

	void ChildComponent::SetRotate(float rotate)
	{
		m_rotate = rotate;
	}

	float ChildComponent::GetRotate() const
	{
		return m_rotate;
	}
}