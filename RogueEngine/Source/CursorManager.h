#include "Vector2D.h"

namespace Rogue
{
	class CursorManager
	{;
	public:
		CursorManager() = default;
		~CursorManager() = default;

		static CursorManager& instance()
		{
			static CursorManager instance;
			return instance;
		}

		void TransformCursorToWorld(Vec2& cursor);
		AABB GenerateMeshAABB(const TransformComponent& trans);
	};

}