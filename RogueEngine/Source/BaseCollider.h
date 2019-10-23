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
		Vec2 getScale() const
		{
			return m_scaleOffset;
		}
		float getRotation() const
		{
			return m_rotationOffset;
		}
		Vec2 getCenter() const
		{
			return m_centerOffset;
		}

		void setScale(const Vec2& scale)
		{
			m_scaleOffset = scale;
		}
		void setRotation(float rot)
		{
			m_rotationOffset = rot;
		}
		void setCenter(const Vec2& center)
		{
			m_centerOffset = center;
		}
	};
}