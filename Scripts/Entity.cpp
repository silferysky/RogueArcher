#include "Entity.h"



Entity::Entity()
{
	ID = LASTENT;
}

Entity::Entity(ENTITYID id)
{
	ID = id;
}

Entity::~Entity()
{
}
