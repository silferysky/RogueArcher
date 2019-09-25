#include "BaseSystem.h"

BaseSystem::BaseSystem()
{
	_ID = LASTSYS;
}

BaseSystem::BaseSystem(SYSTEMID id)
{
	_ID = id;
}


BaseSystem::~BaseSystem()
{
}