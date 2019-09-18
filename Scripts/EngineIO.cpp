#pragma once
#include "EngineIO.h"

std::string EngineIO::ReadFile(std::string FileName)
{
	std::string toReturnStr = std::string();
	std::ifstream iStream(FileName);

	if (iStream.is_open())
	{
		std::string line;

		while (std::getline(iStream, line))
		{
			toReturnStr.append(line);
			toReturnStr.append("\f");
		}
	}
	else
	{
		RE_CORE_ERROR("FILEIO READ: Unable to open file");
	}

	iStream.close();
	return toReturnStr;
}

void EngineIO::WriteFile(std::string FileName, std::string strToWrite)
{
	std::ofstream oStream(FileName);

	if (oStream.is_open())
	{
		oStream << strToWrite;
	}
	else
	{
		RE_CORE_ERROR("FILEIO WRITE: Unable to open file");
	}
	oStream.close();
}
