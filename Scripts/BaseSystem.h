#pragma once

enum class SYSTEMID
{
	MEMORYMANAGER = 0,
	INPUTMANAGER,
	EVENTMANAGER,
	AUDIOMANAGER,
	PHYSICSSYSTEM,
	TESTSYSTEM,
	LASTSYS
};

class BaseSystem
{
public:
	BaseSystem(SYSTEMID ID = SYSTEMID::LASTSYS)
		: _ID{ ID } {};
	~BaseSystem() {};

	SYSTEMID ID() { return _ID; }
	void ID(SYSTEMID id) { _ID = id; }

private:
	SYSTEMID _ID;
};