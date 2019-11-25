#include "Precompiled.h"
#include "PickingSystem.h"
#include "PickingManager.h"
#include "CollisionManager.h"
#include "CameraManager.h"
#include "Main.h"
#include "KeyEvent.h"

namespace Rogue
{
	PickingSystem::PickingSystem()
		: System(SystemID::id_PICKINGSYSTEM)
	{}

	void PickingSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PICKINGSYSTEM, PickingSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<PickingSystem>(signature);
	}

	void PickingSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Picking System");
		

		g_engine.m_coordinator.EndTimeSystem("Picking System");
	}

	void PickingSystem::Shutdown()
	{
	}

	void PickingSystem::Receive(Event* ev)
	{
		// If no entities with transform exist, don't bother.
		if (m_entities.size() == 0)
			return;

		switch (ev->GetEventType())
		{
			// If double click event triggered
		case EventType::EvMouseDoubleClick:
		{	
			MouseDoubleClickEvent* keyTriggered = dynamic_cast<MouseDoubleClickEvent*>(ev);

			// Get the cursor's world position
			Vec2 cursor = g_engine.GetWorldCursor();
			PickingManager::instance().GenerateViewPortAABB(CameraManager::instance().GetCameraPos(), CameraManager::instance().GetCameraZoom());

			// Get the viewport's AABB
			const AABB& viewportArea = PickingManager::instance().GetViewPortArea();

			// If cursor is in the viewport area, proceed.
			if (CollisionManager::instance().DiscretePointVsAABB(cursor, viewportArea))
			{
				int pickedEntity = -1;
				float highestZ = 0;

				// Go through every transform component
				for (Entity entity : m_entities)
				{
					// Skip cursor entities (crosshair)
					if (g_engine.m_coordinator.ComponentExists<CursorComponent>(entity))
						continue;

					TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

					// If entity is in the viewport area
					if (CollisionManager::instance().DiscretePointVsAABB(trans.GetPosition(), viewportArea))
					{	
						// Generate the transform aabb of the entity
						PickingManager::instance().GenerateMeshAABB(trans);
						auto pickArea = trans.GetPickArea();

						// Check if cursor is on the entity
						if (CollisionManager::instance().DiscretePointVsAABB(cursor, pickArea))
						{
							if (trans.GetZ() >= highestZ)
							{
								highestZ = trans.GetZ();
								pickedEntity = entity;
							}
						}
					}
				}

				// Send EntityPickedEvent
				if (pickedEntity >= 0)
				{
					EntPickedEvent* event = new EntPickedEvent{ static_cast<Entity>(pickedEntity) };

					if (g_engine.m_coordinator.GetEditorIsRunning() && !g_engine.m_coordinator.GetGameState())
						event->SetSystemReceivers((int)SystemID::id_EDITOR);

					EventDispatcher::instance().AddEvent(event);
				}
			}
		}
			return;
		} // End switch case
	} // End receive()
} // End namespace Rogue