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

class SystemList
{
public:
	SystemList(SYSTEMID ID = SYSTEMID::LASTSYS)
		: _ID{ ID } {};
	~SystemList() {};

	SYSTEMID ID() { return _ID; }
	void ID(SYSTEMID id) { _ID = id; }

private:
	SYSTEMID _ID;
};