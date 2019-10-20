#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>
#include "FileIO.h"
#include "Logger.h"

namespace Rogue
{
	bool RESerialiser::ReadFromFile(const char* FileName)
	{
		FILE* pFile = nullptr;
		fopen_s(&pFile, FileName, "r");
		if (!pFile)
		{
			return EXIT_FAILURE;
		}
		char buffer[65536];
		rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
		rapidjson::Document document;
		document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
		fclose(pFile);
		return EXIT_SUCCESS;
	}

	rapidjson::Document RESerialiser::DeserialiseFromFile(const char* FileName)
	{
		FILE* pFile = nullptr;
		fopen_s(&pFile, FileName, "r");
		RE_ASSERT(pFile, "Error opening file for deserialization");
		char buffer[65536];
		rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
		fclose(pFile);
		rapidjson::Document document;
		document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
		return document;
	}

	bool RESerialiser::IfFileExists(const char* FileName)
	{
		std::ifstream file(FileName);
		return file.good();
	}

}