#pragma once
#include <string>
#include "RogueEngine.h"

class BasicIO 
{
public:

	//For Read/Write
	static std::string ReadFile(std::string FileName);
	static void WriteFile(std::string FileName, std::string strToWrite);

	//For Json files
	static void WriteJsonFile(std::string FileName, size_t size);

private:

};