#include "Precompiled.h"
#include "CollisionTagSystem.h"
#include "Main.h"
#include "GameEvent.h"

namespace Rogue
{
	CollisionTagSystem::CollisionTagSystem()
		: System(SystemID::id_COLLISIONTAGSYSTEM)
	{}

	void CollisionTagSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_COLLISIONTAGSYSTEM, CollisionTagSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CollisionTagSystem>(signature);

		//Default tag
		AddTag("Unassigned");
		AddTag("Circle");
		AddTag("Box");
	}

	void CollisionTagSystem::Update()
	{
	}

	void CollisionTagSystem::Shutdown()
	{
	}

	void CollisionTagSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvEntityChangeState:
		{
			EntChangeStateEvent* EvChangeState = dynamic_cast<EntChangeStateEvent*>(ev);

			if (g_engine.m_coordinator.ComponentExists<RigidbodyComponent>(EvChangeState->GetEntityID()) &&
				g_engine.m_coordinator.ComponentExists<TransformComponent>(EvChangeState->GetEntityID()))
			{
				if (g_engine.m_coordinator.ComponentExists<BoxCollider2DComponent>(EvChangeState->GetEntityID()))
					AssignTag(EvChangeState->GetEntityID(), "Box");
				else if (g_engine.m_coordinator.ComponentExists<CircleCollider2DComponent>(EvChangeState->GetEntityID()))
					AssignTag(EvChangeState->GetEntityID(), "Circle");
			}
			return;
		}
		}
	}

	void CollisionTagSystem::AddTag(std::string name)
	{
		m_tagsList.emplace_back(name);
	}

	void CollisionTagSystem::RemoveTag(std::string name)
	{
		//Do not ever removed the tag for unassigned
		if (name == "Unassigned")
			return;

		bool deleted = false;
		for (auto iterator = m_tagsList.begin(); iterator != m_tagsList.end(); ++iterator)
		{
			if (*iterator == name)
			{
				iterator = m_tagsList.erase(iterator);
				--iterator;
			}
		}
	}

	void CollisionTagSystem::AssignTag(Entity entityToAssign, std::string tagName)
	{
		//Find the tag based on name
		for (auto iterator = m_tagsList.begin(); iterator != m_tagsList.end(); ++iterator)
		{
			if (*iterator == tagName)
			{
				m_entityTagsMap[entityToAssign] = *iterator;
				break;
			}
		}
	}

	void CollisionTagSystem::DeassignTag(Entity entityToDeassign)
	{
		for (auto iterator = m_entityTagsMap.begin(); iterator != m_entityTagsMap.end(); ++iterator)
		{
			if (iterator->first == entityToDeassign)
			{
				iterator->second = *m_tagsList.begin();
				return;
			}
		}
	}


	std::vector<std::string>& CollisionTagSystem::GetTagList()
	{
		return m_tagsList;
	}

	std::map<Entity, std::string> CollisionTagSystem::GetEntityTagMap()
	{
		return m_entityTagsMap;
	}
}