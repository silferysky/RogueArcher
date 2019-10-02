#include "BaseEntity.h"



BaseEntity::BaseEntity()
{
	ID = LASTENT;
}

BaseEntity::BaseEntity(ENTITYID id)
{
	ID = id;
}

BaseEntity::~BaseEntity()
{
}
