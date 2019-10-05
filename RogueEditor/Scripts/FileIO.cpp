
#include "FileIO.h"

bool ReadFromFile(const char* FileName)
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
	return EXIT_SUCCESS;
}

bool IfFileExists(const char* FileName)
{
	std::ifstream file(FileName);
	return file.good();
}