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