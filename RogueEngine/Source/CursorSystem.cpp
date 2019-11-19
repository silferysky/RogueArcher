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
		
		Vec2 cursorPos;

		CursorManager::instance().TransformCursorToWorld(cursorPos);

		for (Entity entity : m_entities)
		{
			auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
			Vec2 worldCursor(cursorPos);
			g_engine.SetWorldCursor(worldCursor);
			trans.setPosition(worldCursor);
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