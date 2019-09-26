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
		for (std::uint32_t entity = 0; entity < MAX_ENTITIES; ++entity)
		{
			REAvailableEntities.push(entity);
		}
	}

	std::uint32_t CreateEntity()
	{

		// Take an ID from the front of the queue
		std::uint32_t id = REAvailableEntities.front();
		REAvailableEntities.pop();
		++RECurrentEntityCount;
		std::cout << "Entities Created" << std::endl;
		return id;
	}

	void DestroyEntity(std::uint32_t entity)
	{
		std::cout << "Entities Destroyed" << std::endl;
		// Invalidate the destroyed entity's signature
		RESignatures[entity].reset();

		// Put the destroyed ID at the back of the queue
		REAvailableEntities.push(entity);
		--RECurrentEntityCount;
	}

	void SetSignature(std::uint32_t entity, Signature signature)
	{
		RESignatures[entity] = signature;
	}

	Signature GetSignature(std::uint32_t entity)
	{
		return RESignatures[entity];
	}

private:

	std::queue<std::uint32_t> REAvailableEntities{};

	std::array<Signature, MAX_ENTITIES> RESignatures{};

	uint32_t RECurrentEntityCount{};
};