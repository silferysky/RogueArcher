#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"
#include "PhysicsDataStructures.hpp"

namespace Rogue
{
	class RigidbodyComponent
		: public BaseComponent
	{
		Vec2 m_acceleration;
		Vec2 m_velocity;
		Vec2 m_accForce;

		float m_volume;
		float m_damping;

		float m_restitution; // Bounciness
		float m_friction;
		float m_gravityScale;

		bool m_isStatic;

		MassData m_massData;

	public:
		RigidbodyComponent();
		~RigidbodyComponent() = default;

		Vec2 getVelocity() const;
		Vec2 getAccForce() const;
		Vec2 getAcceleration() const;
		float getInvMass() const;
		float getVolume() const;
		float getDamping() const;
		bool getIsStatic() const;
		float getBounciness() const;
		float getFriction() const;
		float getGravityScale() const;

		void setAccForce(const Vec2& accforce);
		void setVelocity(const Vec2& vel);
		void offSetVelocity(const Vec2& vel);
		void setMass(float mass);
		void setVolume(float volume);
		void addForce(const Vec2& force);
		void setAcceleration(const Vec2& accel);
		void offSetAcceleration(const Vec2& accel);
		void setDamping(float damping);
		void setIsStatic(bool set);
		void setBounciness(float bounce);
		void setFriction(float friction);
		void setGravityScale(float gravity);

		void DisplayOnInspector();
		std::string_view Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

	};
}