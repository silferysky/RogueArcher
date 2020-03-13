/* Start Header ************************************************************************/
/*!
\file           EntityManager.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for EntityManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#include "Types.h"
#include "Logger.h"
#include "EditorHierarchyInfo.h"

namespace Rogue
{

	class EntityManager
	{
	private:

		std::queue<Entity> REAvailableEntities{};

		std::array<Signature, MAX_ENTITIES> RESignatures{};

		std::array<HierarchyInfo, MAX_ENTITIES> m_entityInfo;

		std::vector<Entity> m_currentActiveObjects;

		uint32_t REActiveEntityCount{};

	public:
		EntityManager()
		{
			Init();
		}
		
		void Init()
		{
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				REAvailableEntities.push(entity);
			}
		}

		Entity CreateEntity()
		{
			// Take an ID from the front of the queue
			if (REActiveEntityCount > MAX_ENTITIES)
			{
				REActiveEntityCount = static_cast<uint32_t>(m_currentActiveObjects.size());
			}

			RE_ASSERT(REActiveEntityCount < MAX_ENTITIES, "Too many entities in existence.");

			Entity id = REAvailableEntities.front();
			REAvailableEntities.pop();
			++REActiveEntityCount;
			//std::cout << REActiveEntityCount << std::endl;

			//std::stringstream out;
			//out << "Entities created. Current active entities: " << REActiveEntityCount;
			//////RE_CORE_INFO(out.str());

			return id;
		}

		void DestroyEntity(Entity entity)
		{
			RE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");

			// Invalidate the destroyed entity's signature
			RESignatures[entity].reset();

			// Put the destroyed ID at the back of the queue
			RemoveEntityFromActiveObjects(entity);
			REAvailableEntities.push(entity);
			--REActiveEntityCount;

#if ENABLE_CORE_LOGGING
			std::stringstream out;
			out << "Entities Destroyed. Current active entities: " << REActiveEntityCount;
			RE_CORE_INFO(out.str());
#endif
		}

		void RemoveEntityFromActiveObjects(Entity entity)
		{
			m_currentActiveObjects.erase(std::remove(m_currentActiveObjects.begin(), m_currentActiveObjects.end(), entity),
				m_currentActiveObjects.end());
		}

		void SetSignature(Entity entity, Signature signature)
		{
			RE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			RESignatures[entity] = signature;
		}

		Signature GetSignature(Entity entity)
		{

			RE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			return RESignatures[entity];
		}

		HierarchyInfo& GetHierarchyInfo(Entity entity)
		{
			RE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			
			return m_entityInfo[entity];
		}

		std::array<HierarchyInfo, MAX_ENTITIES>& GetHierarchyInfoArray()
		{
			return m_entityInfo;
		}

		void RemoveHierarchyInfo(Entity ent)
		{
			m_entityInfo[ent] = HierarchyInfo();
		}

		uint32_t GetActiveEntityCount()
		{
			return REActiveEntityCount;
		}

		std::vector <Entity>& m_getActiveObjects()
		{
			return m_currentActiveObjects;
		}

		void ResetEntityQueue()
		{
			while (REAvailableEntities.size())
				REAvailableEntities.pop();

			Init();
		}
	};
}
