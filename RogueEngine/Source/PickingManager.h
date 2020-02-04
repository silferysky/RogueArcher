/*Start Header************************************************************************ /
/*!
\file           PickingManager.h
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for PickingManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
		Vec2 m_cursorPos;				// Mouse cursor's world position
		ImVec2 m_viewportCursorPos;		// Mouse cursor's editor viewport position

	public:
		PickingManager() = default;
		~PickingManager() = default;

		static PickingManager& instance()
		{
			static PickingManager instance;
			return instance;
		}

		void TransformCursorToWorld(const Vec2& cursor);
		void GenerateMeshAABB(TransformComponent& trans) const;
		void GenerateViewPortAABB(const glm::vec3& cameraPos, float zoom);
		
		const AABB& GetViewPortArea() const;
		const Vec2& GetWorldCursor() const;
		const ImVec2& GetViewportCursor() const;

		void SetViewPortArea(const AABB& aabb);
		void SetWorldCursor(const Vec2& pos);
		void SetViewPortCursor(const ImVec2& pos);

		bool isCursorinViewPort() const;
	};

}