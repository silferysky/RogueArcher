#pragma once
#include "../Scripts/REMath.h"

class RigidbodyComponent
{
	Vec2 m_acceleration;
	Vec2 m_velocity;
	Vec2 m_accForce;

	float m_invMass;
	float m_volume;
	float m_damping;

public:
	RigidbodyComponent(float mass = 1.0f,
			  float volume = 1.0f,
			  float damping = 0.99f);
	~RigidbodyComponent() = default;

	Vec2 getVelocity() const;
	Vec2 getAccForce() const;
	Vec2 getAcceleration() const;
	float getInvMass() const;
	float getVolume() const;
	float getDamping() const;

	void setAccForce(const Vec2& accforce);
	void setVelocity(const Vec2& vel);
	void offSetVelocity(const Vec2& vel);
	void setMass(float mass);
	void setVolume(float volume);
	void addForce(const Vec2& force);
	void setAcceleration(const Vec2& accel);
	void offSetAcceleration(const Vec2& accel);
	void setDamping(float damping);

};