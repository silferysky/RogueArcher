#include "System.h"
#include "Main.h"

System::System()
{
	ID = LASTSYS;
}

System::System(SYSTEMID id)
{
	ID = id;
	if (ID != MEMORYMANAGER && SysManager != nullptr)
	{
	}
}


System::~System()
{
}