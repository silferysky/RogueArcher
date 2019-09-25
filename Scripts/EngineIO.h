#pragma once
#include <fstream>
#include <string>
#include "RogueEngine.h"

class EngineIO 
{
public:

	//For Read/Write
	static std::string ReadFile(std::string FileName);
	static void WriteFile(std::string FileName, std::string strToWrite);

private:

};