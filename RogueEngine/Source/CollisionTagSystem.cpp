#include "CollisionTagSystem.h"
#include "Main.h"

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
	}

	void CollisionTagSystem::Update()
	{
	}

	void CollisionTagSystem::Shutdown()
	{
	}

	void CollisionTagSystem::Receive(Event* ev)
	{
	}

	void CollisionTagSystem::AddTag(std::string name)
	{
		CollisionTag collisionTag(name, m_tagsList.size());
		m_tagsList.push_back(collisionTag);
	}

	void CollisionTagSystem::RemoveTag(std::string name)
	{
		//Do not ever removed the tag for unassigned
		if (name == "Unassigned")
			return;

		bool deleted = false;
		for (auto iterator = m_tagsList.begin(); iterator != m_tagsList.end(); ++iterator)
		{
			//If deleted tag, make sure all other tags decrement by 1 to match properly
			if (deleted)
			{
				iterator->m_tag--;
			}
			else if (iterator->m_name == name)
			{
				iterator = m_tagsList.erase(iterator);
				--iterator;
				deleted = true;
			}
		}
	}

	void CollisionTagSystem::RemoveTag(int tag)
	{
		//Do not ever removed the tag for unassigned
		if (!tag)
			return;

		bool deleted = false;
		for (auto iterator = m_tagsList.begin(); iterator != m_tagsList.end(); ++iterator)
		{
			//If deleted tag, make sure all other tags decrement by 1 to match properly
			if (deleted)
			{
				iterator->m_tag--;
			}
			else if (iterator->m_tag == tag)
			{
				iterator = m_tagsList.erase(iterator);
				deleted = true;
			}
		}
	}

	void CollisionTagSystem::AssignTag(Entity entityToAssign, std::string tagName)
	{
		//Find the tag based on name
		for (auto iterator = m_tagsList.begin(); iterator != m_tagsList.end(); ++iterator)
		{
			if (iterator->m_name == tagName)
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
}