#include "Precompiled.h"
#include "CursorManager.h"
#include "Main.h"

namespace Rogue
{
	void CursorManager::TransformCursorToWorld(Vec2& cursorPos) const
	{
		// Windows cursor
		POINT cursor;

		if (g_engine.m_coordinator.GetEditorIsRunning())
			cursorPos = g_engine.GetViewportCursor();

		else if (GetCursorPos(&cursor))
		{
			ScreenToClient(g_engine.GetWindowHandler(), &cursor);
			cursorPos.x = static_cast<float>(cursor.x);
			cursorPos.y = static_cast<float>(cursor.y);
		}

		float x = (2.0f * cursorPos.x) / GetWindowWidth(g_engine.GetWindowHandler()) - 1.0f;
		float y = 1.0f - (2.0f * cursorPos.y) / GetWindowHeight(g_engine.GetWindowHandler());
		float z = 1.0f;

		glm::vec3 rayNDC = glm::vec3(x, y, z);

		glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

		glm::vec4 rayEye = glm::inverse(g_engine.GetProjMat()) * rayClip;

		glm::mat4 viewMat = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetViewMatrix(1.0f);

		glm::vec4 rayWorld4D = glm::inverse(viewMat) * rayEye;

		cursorPos = Vec2{ rayWorld4D.x, rayWorld4D.y };
	}

	void CursorManager::GenerateMeshAABB(TransformComponent& trans) const
	{
		AABB pickArea;
		Vec2 pos = trans.GetPosition();
		Vec2 scale = trans.GetScale();

		pickArea.setMin(Vec2{ pos.x - scale.x * 0.5f, pos.y - scale.y * 0.5f });
		pickArea.setMax(Vec2{ pos.x + scale.x * 0.5f, pos.y + scale.y * 0.5f });

		trans.setPickArea(pickArea);
	}

	void CursorManager::GenerateViewPortArea()
	{

	}
}