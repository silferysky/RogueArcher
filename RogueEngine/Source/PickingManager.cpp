#include "Precompiled.h"
#include "PickingManager.h"

namespace Rogue
{
	Vec2 PickingManager::TransformCursorToWorld(const Vec2& cursorPos) const
	{
		float x = (2.0f * cursorPos.x) / GetWindowWidth(g_engine.GetWindowHandler()) - 1.0f;
		float y = 1.0f - (2.0f * cursorPos.y) / GetWindowHeight(g_engine.GetWindowHandler());
		float z = 1.0f;

		glm::vec3 rayNDC = glm::vec3(x, y, z);

		glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

		glm::vec4 rayEye = glm::inverse(g_engine.GetProjMat()) * rayClip;

		glm::mat4 viewMat = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetViewMatrix(1.0f);

		glm::vec4 rayWorld4D = glm::inverse(viewMat) * rayEye;

		return Vec2{ rayWorld4D.x, rayWorld4D.y };
	}

	void PickingManager::GenerateMeshAABB(TransformComponent& trans) const
	{
		AABB pickArea;
		Vec2 pos = trans.GetPosition();
		Vec2 scale = trans.GetScale();
		
		scale.x = REAbs(scale.x);
		scale.y = REAbs(scale.y);

		pickArea.setMin(Vec2{ pos.x - scale.x * 0.5f, pos.y - scale.y * 0.5f });
		pickArea.setMax(Vec2{ pos.x + scale.x * 0.5f, pos.y + scale.y * 0.5f });

		trans.setPickArea(pickArea);
	}

	void PickingManager::GenerateViewPortArea(const Vec2& min, const Vec2& max)
	{
		m_viewportArea = AABB{ min, max };
	}

	const AABB& PickingManager::GetViewPortArea() const
	{
		return m_viewportArea;
	}

	Entity PickingManager::ChooseTopLayer() const
	{
		// Return the last element of the set.
		return *m_pickedEntities.rbegin();
	}

	void PickingManager::SetViewPortArea(const AABB& aabb)
	{
		m_viewportArea = aabb;
	}

	void PickingManager::AddPickedEntity(Entity entity)
	{
		m_pickedEntities.emplace(entity);
	}
}