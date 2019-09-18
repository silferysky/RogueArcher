#pragma once
class ColliderComponent
{
	AABB m_alignedBox;
	OBB  m_orientedBox;

public:
	ColliderComponent();
	virtual ~ColliderComponent();

	friend class PhysicsSystem;
};

