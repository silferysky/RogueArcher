#pragma once
#include <vector>

enum ShapeType // List of convex hulls
{
	SHAPE_NONE,
	SHAPE_TRI = 3,
	SHAPE_RECT,
	SHAPE_PENTAGON
};


class AABB
{
	Vec2 min;
	Vec2 max;

public:
	AABB() : min{ 0 }, max{ 0 }
	{}
	~AABB()
	{}

	Vec2 getMin() const;
	Vec2 getMax() const;
	void setMin(const float x, const float y);
	void setMax(const float x, const float y);

	bool CollisionTest(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2);
	inline bool staticCollision(const AABB &rhs);
	bool movingCollision(const AABB &rhs, const Vec2 &vel1, const Vec2 &vel2);
};


class OBB
{
	Vec2 center;
	float angle;
	float scale;
	float minOnAxis; // To be used to calculate collision
	float maxOnAxis;

	ShapeType sides; // Number of sides/faces.
	std::vector<Vec2> relativeVerts; // Corners relative to center.
	std::vector<Vec2> vertices; // The positions of each vertex in the shape.
	std::vector<Vec2> normals; // The normal vectors of each side.

public:
	OBB();

	~OBB(); // Destructor
	
	void setShape(const ShapeType newShape);
	void createShape(const std::vector<Vec2> &corners);

    void updateOBB(const Vec2 pos, const float rot, const float size);
	void updateVertices();
	inline void updateNormals();

	void SATFindMinMax(const Vec2 &normal);
	bool CollisionTest(OBB rhs);


	bool overlaps(const OBB &rhs) const;
	inline bool isBetweenBounds(float val, float lowerBound, float upperBound) const;
};


/*******************************************************************************

					   n1
					   ^
					   |
					   |
				p1 ^-------> p2
				   |       |
        n4 <------ |   C   | -------> n2
				   |       |
				p4 <-------v p3
				       |
					   |
					   v
					   n3


********************************************************************************/