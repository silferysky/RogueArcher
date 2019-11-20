#pragma once
#include "Vector2D.h"
#include "AABB.h"
#include "Types.h"
#include "TransformComponent.h"
#include "Main.h"

namespace Rogue
{
	class LayerComparer
	{
	public:
		bool operator()(Entity entityA, Entity entityB) const
		{
			TransformComponent& transA = g_engine.m_coordinator.GetComponent<TransformComponent>(entityA);
			TransformComponent& transB = g_engine.m_coordinator.GetComponent<TransformComponent>(entityB);
			return transA.GetZ() < transB.GetZ();
		}
	};

	class PickingManager
	{
		AABB m_viewportArea;
		std::set<Entity, LayerComparer> m_pickedEntities;
	public:
		PickingManager() = default;
		~PickingManager() = default;

		static PickingManager& instance()
		{
			static PickingManager instance;
			return instance;
		}

		Vec2 TransformCursorToWorld(const Vec2& cursor) const;
		void GenerateMeshAABB(TransformComponent& trans) const;
		void GenerateViewPortArea(const Vec2& min, const Vec2& max);
		
		const AABB& GetViewPortArea() const;
		Entity ChooseTopLayer() const;

		void SetViewPortArea(const AABB& aabb);
		void AddPickedEntity(Entity entity);
	};

}