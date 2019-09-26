#include "BaseSystem.h"

BaseSystem::BaseSystem()
{
	ID = LASTSYS;
}

BaseSystem::BaseSystem(SYSTEMID id)
{
	ID = id;
}


BaseSystem::~BaseSystem()
{
}