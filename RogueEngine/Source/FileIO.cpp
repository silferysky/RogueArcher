/* Start Header ************************************************************************/
/*!
\file           FileIO.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for FileIO

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "FileIO.h"
#include "Logger.h"

#define MAX_FILE_BUFFER_SIZE 1048575

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
		std::vector<char> buffer(MAX_FILE_BUFFER_SIZE);
		rapidjson::FileReadStream is(pFile, buffer.data(), MAX_FILE_BUFFER_SIZE);
		rapidjson::Document document;
		document.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);
		fclose(pFile);
		return EXIT_SUCCESS;
	}

	rapidjson::Document RESerialiser::DeserialiseFromFile(const char* FileName)
	{
		std::FILE* pFile = nullptr;
		fopen_s(&pFile, FileName, "r");

		RE_ASSERT(pFile, "Error opening file for deserialization");

		std::vector<char> buffer(MAX_FILE_BUFFER_SIZE);
		rapidjson::FileReadStream is(pFile, buffer.data(), MAX_FILE_BUFFER_SIZE);
		
		if(pFile)
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