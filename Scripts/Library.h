#pragma once
#include "EngineIO.h"
#include <sstream>
#include <vector>

enum LibraryID
{
	//For Specifically individual segments
	ComponentID = 0,
	EntityID,
	SystemID,

	//For specifically prefab-type segments
	ComponentData,
	EntityData,
	SystemData,

	//For testing purposes
	FileIOTest,
	PlayerStatsData
};

class Library
{
public:
	Library();
	~Library();

	//For Reading into Library
	void IntepretIO(LibraryID id, std::string data);

	//Funcs to help get the string or ID or file
	std::string GetFileName(LibraryID id);

	//For testing
	void PrintData(std::string dataToPrint);
	void IOTest();

private:
	//std::list<Component> ComponentTemplate;
	//std::list<Entity> EntityTemplate;
	//std::list<System> SystemTemplate;
};

#define LIBRARY_READ_INTEPRET(id) IntepretIO(id, EngineIO::ReadFile(GetFileName(id)));
#define LIBRARY_WRITE(id, string) EngineIO::WriteFile(GetFileName(id), string);