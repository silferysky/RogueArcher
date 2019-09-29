#pragma once
#include "Types.h"
#include <queue>
#include <bitset>
#include <array>
#include "Logger.h"

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
		RESignatures[entity] = signature;
	}

	Signature GetSignature(Entity entity)
	{
		return RESignatures[entity];
	}

private:

	std::queue<Entity> REAvailableEntities{};

	std::array<Signature, MAX_ENTITIES> RESignatures{};

	uint32_t REActiveEntityCount{};
};