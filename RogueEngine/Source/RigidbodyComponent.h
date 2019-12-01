/* Start Header ************************************************************************/
/*!
\file           RigidBodyComponent.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for RigidBodyComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		float getMass() const;
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
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

	};
}