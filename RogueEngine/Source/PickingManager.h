#pragma once
#include "Vector2D.h"
#include "AABB.h"
#include "Types.h"
#include "TransformComponent.h"
#include "Main.h"

namespace Rogue
{
	class PickingManager
	{
		AABB m_viewportArea;
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
		void GenerateViewPortAABB(const glm::vec3& cameraPos, float zoom);
		
		const AABB& GetViewPortArea() const;
		void SetViewPortArea(const AABB& aabb);
	};

}