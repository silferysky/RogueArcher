#include "Entity.h"



Entity::Entity()
{
	ID = LASTENT;
	nextEnt = nullptr;
}

Entity::Entity(ENTITYID id)
{
	ID = id;
	nextEnt = nullptr;
}

Entity::~Entity()
{
}
