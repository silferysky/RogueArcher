#pragma once
#include "Types.h"
#include <queue>
#include <bitset>
#include <array>

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
		++RECurrentEntityCount;
		std::cout << "Entities Created" << std::endl;
		return id;
	}

	void DestroyEntity(Entity entity)
	{
		std::cout << "Entities Destroyed" << std::endl;
		// Invalidate the destroyed entity's signature
		RESignatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		REAvailableEntities.push(entity);
		--RECurrentEntityCount;
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

	uint32_t RECurrentEntityCount{};
};