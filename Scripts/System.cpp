#include "System.h"
#include "Main.h"

System::System()
{
	ID = LASTSYS;
	nextSys = nullptr;
}

System::System(SYSTEMID id)
{
	ID = id;
	nextSys = nullptr;
	if (ID != MEMORYMANAGER && SysManager != nullptr)
	{
	}
}


System::~System()
{
}