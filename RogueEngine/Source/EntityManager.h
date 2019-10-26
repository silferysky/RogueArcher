#pragma once
#include <cassert>
#include <sstream>
#include <queue>
#include <bitset>
#include <array>

#include "Types.h"
#include "Logger.h"

namespace Rogue
{
	class EntityManager
	{
	public:
		EntityManager()
		{
			for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
			{
				REAvailableEntities.push(entity);
			}
		}

		Entity CreateEntity()
		{
			// Take an ID from the front of the queue
			RE_ASSERT(REActiveEntityCount < MAX_ENTITIES, "Too many entities in existence.");
			Entity id = REAvailableEntities.front();
			REAvailableEntities.pop();
			++REActiveEntityCount;

			std::stringstream out;
			out << "Entities created. Current active entities: " << REActiveEntityCount;
			RE_CORE_INFO(out.str());

			return id;
		}

		void DestroyEntity(Entity entity)
		{
			RE_ASSERT(entity < MAX_ENTITIES, "Entity out of range.");
			// Invalidate the destroyed entity's signature
			RESignatures[entity].reset();

			// Put the destroyed ID at the back of the queue
			REAvailableEntities.push(entity);
			--REActiveEntityCount;

			std::stringstream out;
			out << "Entities Destroyed. Current active entities: " << REActiveEntityCount;
			RE_CORE_INFO(out.str());
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

		uint32_t GetActiveEntityCount()
		{
			return REActiveEntityCount;
		}

	private:

		std::queue<Entity> REAvailableEntities{};

		std::array<Signature, MAX_ENTITIES> RESignatures{};

		uint32_t REActiveEntityCount{};
	};
}
