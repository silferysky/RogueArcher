#pragma once
#include "REMath.h"

namespace Rogue
{
	class BaseCollider
	{
		// Collider matrix info (Offset from its mesh)
		Vec2 m_scaleOffset;
		Vec2 m_centerOffset;
		float m_rotationOffset;

	public:
		BaseCollider() :
			m_scaleOffset{ Vec2{} }, m_centerOffset{ Vec2{} }, m_rotationOffset{ 0.0f }
		{}

		virtual ~BaseCollider() = default;

		BaseCollider(const BaseCollider& rhs) = default;

		BaseCollider(BaseCollider&& rhs) noexcept :
			m_scaleOffset{ Vec2{} }, m_centerOffset{ Vec2{} }, m_rotationOffset{ 0.0f }
		{
			std::swap(m_scaleOffset, rhs.m_scaleOffset);
			std::swap(m_centerOffset, rhs.m_centerOffset);
			std::swap(m_rotationOffset, rhs.m_rotationOffset);
		}

		BaseCollider& operator=(const BaseCollider& rhs)
		{
			if (this != &rhs)
			{
				m_scaleOffset = rhs.m_scaleOffset;
				m_centerOffset = rhs.m_centerOffset;
				m_rotationOffset = rhs.m_rotationOffset;
			}
			return *this;
		}

		BaseCollider& operator=(BaseCollider&& rhs) noexcept
		{
			if (this != &rhs)
			{
				std::swap(m_scaleOffset, rhs.m_scaleOffset);
				std::swap(m_centerOffset, rhs.m_centerOffset);
				std::swap(m_rotationOffset, rhs.m_rotationOffset);
			}

			return *this;
		}

		Vec2 getScaleOffSet() const
		{
			return m_scaleOffset;
		}
		float getRotationOffSet() const
		{
			return m_rotationOffset;
		}
		Vec2 getCenterOffSet() const
		{
			return m_centerOffset;
		}

		void setScaleOffSet(const Vec2& scale)
		{
			m_scaleOffset = scale;
		}
		void setRotationOffSet(float rot)
		{
			m_rotationOffset = rot;
		}
		void setCenterOffSet(const Vec2& center)
		{
			m_centerOffset = center;
		}
	};
}