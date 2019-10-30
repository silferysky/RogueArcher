#pragma once
#include "REMath.h"
#include "AABB.h"
#include "OBB.h"

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
	
		CircleShape(float radius = 1.0f) :
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
		const AABB& m_aabb;

		BoxShape(const AABB& aabb = AABB()) :
			m_aabb{ aabb }
		{}

		const MassData& ComputeMass(float density = 1.0f) const override
		{
			float width = m_aabb.getMax().x - m_aabb.getMin().x;
			float height = m_aabb.getMax().y - m_aabb.getMax().y;
			float mass = width * height * density;

			return MassData{
				mass,
				mass * (width * width + height * height) / 12 }; // Formula for inertia of box.
		}

		Type GetType() const override
		{
			return Type::e_box;
		}
	};

	class PolygonShape : public Shape
	{
		const OBB::VertexList& m_vertices;
	public:
		PolygonShape() = default;

		PolygonShape(OBB& obb) :
			m_vertices{ obb.modelVerts() }
		{}

		const MassData& ComputeMass(float density) const override
		{
			size_t m_vertexCount = m_vertices.size();

			// Calculate centroid and moment of interia
			Vec2 c(0.0f, 0.0f); // centroid
			float area = 0.0f;
			float I = 0.0f;
			const float k_inv3 = 1.0f / 3.0f;
			float D = 1.0f;

			for (unsigned int i1 = 0; i1 < m_vertexCount; ++i1)
			{
				// Triangle vertices, third vertex implied as (0, 0)
				Vec2 p1(m_vertices[i1]);
				unsigned int i2 = i1 + 1 < m_vertexCount ? i1 + 1 : 0;
				Vec2 p2(m_vertices[i2]);

				//	float D = Cross(p1, p2); // Cross product
				float triangleArea = 0.5f * D;

				area += triangleArea;

				// Use area to weight the centroid average, not just vertex position
				c += triangleArea * k_inv3 * (p1 + p2);

				float intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
				float inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
				I += (0.25f * k_inv3 * D) * (intx2 + inty2);
			}

			c *= 1.0f / area;

			// Translate vertices to centroid (make the centroid (0, 0)
			// for the polygon in model space)
			//for (unsigned int i = 0; i < m_vertexCount; ++i)
			//	m_vertices[i] = m_vertices[i] - c;

			float mass = density * area;
			float inertia = I * density;

			return MassData{ mass, inertia };
		}

		Type GetType() const override
		{
			return Type::e_polygon;
		}
	};

} // ns Rogue