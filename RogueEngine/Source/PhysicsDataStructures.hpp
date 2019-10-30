#pragma once
#include "REMath.h"

namespace Rogue
{
	enum class BodyType
	{
		e_static,		// Does not move, collides with DYNAMIC
		e_kinematic,	// Moves by itself, collides with DYNAMIC
		e_dynamic,	// Collides with all, requires collision response

		e_max
	};

	enum class CollisionMode
	{
		e_awake,
		e_trigger,
		e_asleep,

		e_max
	};

	struct MassData
	{
		float m_mass;
		float m_inertia;

		float m_invMass;
		float m_invInertia;

		MassData(float mass = 1.0f, float inertia = 1.0f) :
			m_mass{ mass }, m_inertia{ inertia },
			m_invMass{ mass ? 1.0f / mass : 0.0f }, m_invInertia{ inertia ? 1.0f / mass : 0.0f }
		{}
	};

	// Class to define the shape's characteristics
	class Shape
	{
	protected:
		enum class Type
		{
			e_circle,
			e_box,
			e_polygon,

			e_max
		};

	public:
		virtual const MassData& ComputeMass(float density) const = 0;
		virtual Type GetType() const = 0;
	};

	class CircleShape : public Shape
	{
	public:
		float m_radius;
	
		CircleShape(float radius) :
			m_radius{ radius }
		{}

		const MassData& ComputeMass(float density = 1.0f) const override
		{
			float mass = RE_PI * m_radius * m_radius * density;
			float inertia = mass * m_radius * m_radius;

			return MassData{ mass, inertia };
		}

		Type GetType() const override
		{
			return Type::e_circle;
		}
	};

	class BoxShape : public Shape
	{
	public:
		float m_width;
		float m_height;

		BoxShape(float width, float height) :
			m_width{ width }, m_height{ height }
		{}

		const MassData& ComputeMass(float density = 1.0f) const override
		{
			float mass = m_width * m_height * density;

			return MassData{
				mass,
				mass * (m_width * m_width + m_height * m_height) / 12 }; // Formula for inertia of box.
		}

		Type GetType() const override
		{
			return Type::e_box;
		}
	};

} // ns Rogue