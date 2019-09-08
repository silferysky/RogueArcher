#pragma once

class SystemManager;

enum SYSTEMID
{
	MEMORYMANAGER = 0,
	INPUTMANAGER,
	EVENTMANAGER,
	AUDIOMANAGER,
	LASTSYS
};

class System
{
public:
	System();
	System(SYSTEMID ID);
	~System();

private:
	SYSTEMID ID;
};