#pragma once

//class SystemManager;

enum SYSTEMID
{
	MEMORYMANAGER = 0,
	INPUTMANAGER,
	EVENTMANAGER,
	AUDIOMANAGER,
	TESTSYSTEM,
	LASTSYS
};

class BaseSystem
{
public:
	BaseSystem() { _ID = LASTSYS; };
	BaseSystem(SYSTEMID ID) {_ID = ID;}
	~BaseSystem() {};

	SYSTEMID ID() { return _ID; }
	void ID(SYSTEMID id) { _ID = id; }

private:
	SYSTEMID _ID;
};