#pragma once
#include <fstream>
#include <string>
#include "RogueEngine.h"

enum FileIOID
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

class EngineIO 
{
public:
	EngineIO();
	~EngineIO();

	//FOr Read/Write
	void ReadFile(FileIOID id);
	void WriteFile(FileIOID id);

protected:
	//Funcs to help get the string or ID or file
	const char * GetFileName(FileIOID id);

private:

};