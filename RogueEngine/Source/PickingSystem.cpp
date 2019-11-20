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
		case EventType::EvKeyTriggered:
			KeyTriggeredEvent* keyTriggered = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keyTriggered->GetKeyCode();

			if (keycode == KeyPress::MB1)
			{
				Vec2 cursor = g_engine.GetWorldCursor();
				const AABB& viewportArea = PickingManager::instance().GetViewPortArea();

				// If cursor is in the viewport area, proceed.
				if (CollisionManager::instance().DiscretePointVsAABB(cursor, viewportArea))
				{
					for (Entity entity : m_entities)
					{
						TransformComponent& trans = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

						// If entity is in the viewport area, generate the transform aabb of the entity
						if (CollisionManager::instance().DiscretePointVsAABB(trans.GetPosition(), viewportArea))
						{
							PickingManager::instance().GenerateMeshAABB(trans);

							// Check if cursor is on the entity
							if (CollisionManager::instance().DiscretePointVsAABB(cursor, trans.GetPickArea()))
							{
								// If true, add to the set of potentially picked entities
								PickingManager::instance().AddPickedEntity(entity);
							}
						}
					}
					Entity pickedEntity = PickingManager::instance().ChooseTopLayer();

					// Send EntityPickedEvent
					std::cout << "Entity picked: " << pickedEntity << std::endl;
				}
			}
			return;
		} // End switch case
	} // End receive()
} // End namespace Rogue