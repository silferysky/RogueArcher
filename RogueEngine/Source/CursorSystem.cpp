/*Start Header************************************************************************ /
/*!
\file           CursorSystem.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for CursorSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "CursorSystem.h"
#include "PickingManager.h"
#include "CollisionManager.h"
#include "CameraManager.h"
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

		// Windows cursor
		POINT windowCursor;
		Vec2 cursorPos;


		if (g_engine.m_coordinator.GetEditorIsRunning())
			cursorPos = PickingManager::instance().GetViewportCursor();

		else if (GetCursorPos(&windowCursor))
		{
			ScreenToClient(g_engine.GetWindowHandler(), &windowCursor);
			cursorPos.x = static_cast<float>(windowCursor.x);
			cursorPos.y = static_cast<float>(windowCursor.y);
		}

		// Calculate cursor's world position and save it.
		PickingManager::instance().TransformCursorToWorld(cursorPos);


		for (Entity entity : m_entities)
		{
			// Set the cursor entity's position to be the cursor's world position
			auto& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			trans.setPosition(PickingManager::instance().GetWorldCursor());
		}

		g_engine.m_coordinator.EndTimeSystem("Cursor System");
	}

	void CursorSystem::Shutdown()
	{
	}

	void CursorSystem::Receive(Event& ev)
	{
	}
}