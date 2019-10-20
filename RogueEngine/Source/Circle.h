#pragma once


namespace Rogue
{
	// Circle class
	class Circle
	{
		float m_radius;

	public:
		Circle(float radius);
		~Circle();

		inline void setRadius(float rad);
		inline float getRadius() const;
	};
}