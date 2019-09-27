#pragma once
#include "Vector2D.h"


class Rigidbody // : public BaseComponent
{
	Vec2 m_velocity;
	Vec2 m_force;

	float m_mass;
	float m_volume;
	float m_orientation;

public:
	Rigidbody() = default;
	~Rigidbody() = default;

	void computeForce(const float mass, const Vec2& gravity);

	inline Vec2 getVelocity() const;
	inline Vec2 getForce() const;
	inline float getMass() const;
	inline float getVolume() const;
	inline float getAngle() const;

	inline void setVelocity(const Vec2& vel);
	inline void setMass(float mass);
	inline void setVolume(float volume);
	inline void setAngle(float angle);
	inline void setForce(const Vec2& force);
};