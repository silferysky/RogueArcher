#include "System.h"
#include "Main.h"

System::System()
{
}

System::System(SYSTEMID ID)
{
	this->ID = ID;
	if (ID != MEMORYMANAGER && SysManager != nullptr)
	{
	}
}


System::~System()
{
}