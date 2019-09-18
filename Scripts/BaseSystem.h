#pragma once

//class SystemManager;

enum SYSTEMID
{
	MEMORYMANAGER = 0,
	INPUTMANAGER,
	EVENTMANAGER,
	AUDIOMANAGER,
	LASTSYS
};

class BaseSystem
{
public:
	BaseSystem();
	BaseSystem(SYSTEMID ID);
	~BaseSystem();

private:
	SYSTEMID ID;
};