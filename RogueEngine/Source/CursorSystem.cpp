#include "Precompiled.h"
#include "CursorSystem.h"
#include "Main.h"
#include "GameEvent.h"

namespace Rogue
{
	CursorSystem::CursorSystem()
		: System(SystemID::id_CURSORSYSTEM)
	{}

	void CursorSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_CURSORSYSTEM, CursorSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<CursorComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CursorSystem>(signature);
	}

	void CursorSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Cursor System");

		// Get Cursor position
		POINT cursor;
		Vec2 cursorPos;

		if (GetCursorPos(&cursor))
		{
			ScreenToClient(g_engine.GetWindowHandler(), &cursor);
			cursorPos.x = static_cast<float>(cursor.x);
			cursorPos.y = static_cast<float>(cursor.y);
		}

		float x = (2.0f * cursorPos.x) / GetWindowWidth(g_engine.GetWindowHandler()) - 1.0f;
		float y = 1.0f - (2.0f * cursorPos.y) / GetWindowHeight(g_engine.GetWindowHandler());
		float z = 1.0f;

		glm::vec3 rayNDC = glm::vec3(x, y, z);

		// convert from ndc to viewport coordinates if editor is on

		glm::vec4 rayClip = glm::vec4(rayNDC.x, rayNDC.y, -1.0f, 1.0f);

		glm::vec4 rayEye = glm::inverse(g_engine.GetProjMat()) * rayClip;

		glm::mat4 viewMat = g_engine.m_coordinator.GetSystem<CameraSystem>()->GetViewMatrix(1.0f);

		glm::vec4 rayWorld4D = glm::inverse(viewMat) * rayEye;

		g_engine.SetWorldCursor(Vec2{ rayWorld4D.x, rayWorld4D.y });

		if (g_engine.m_coordinator.GetEditorIsRunning())
		{
			for (Entity entity : m_entities)
			{
				auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
				trans.setPosition(g_engine.GetViewportCursor());
			}
		}
		else
		{
			for (Entity entity : m_entities)
			{
				auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
				trans.setPosition(g_engine.GetWorldCursor());
			}
		}
		g_engine.m_coordinator.EndTimeSystem("Cursor System");
	}

	void CursorSystem::Shutdown()
	{
	}

	void CursorSystem::Receive(Event* ev)
	{
	}
}