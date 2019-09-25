#pragma once
#include "REMath.h"

class Transform // : public BaseComponent
{
	Vec2 m_position;
	Vec2 m_scale;
	float m_rotation;

public:
	void setPosition(const Vec2& pos);
	void offSetPosition(const Vec2& pos);
	void setScale(const Vec2& scale);
	void offSetScale(const Vec2& scale);
	void setRotation(const float rot);
	void offSetRotation(const float rot);

	Vec2 getPosition() const;
	Vec2 getScale() const;
	float getRotation() const;
};

