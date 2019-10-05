#include "BoxCollider2DComponent.h"

AABB& BoxCollider2DComponent::AABB()
{
	return m_aabb;
}

OBB& BoxCollider2DComponent::OBB()
{
	return m_obb;
}
