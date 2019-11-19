#pragma once
#include "Vector2D.h"
#include "AABB.h"

namespace Rogue
{
	class TransformComponent;

	class CursorManager
	{
		AABB m_viewportArea;
	public:
		CursorManager() = default;
		~CursorManager() = default;

		static CursorManager& instance()
		{
			static CursorManager instance;
			return instance;
		}

		void TransformCursorToWorld(Vec2& cursor) const;
		void GenerateMeshAABB(TransformComponent& trans) const;
		void GenerateViewPortArea();
	};

}