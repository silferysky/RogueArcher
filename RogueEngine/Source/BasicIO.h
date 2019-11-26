#pragma once
#include <string>

namespace Rogue
{
	class BasicIO
	{
	public:

		//For Read/Write
		static std::string ReadFile(std::string FileName);
		static void WriteFile(std::string FileName, std::string strToWrite);

		//For Json files
		static void WriteLevelFilesJsonFile(std::string_view FileName);
		static void WriteLevelJsonFile(std::string_view FileName, size_t numOfEnt);
		static void WriteArchetypeJsonFile(std::string_view FileName);
		static void WriteArchetypeListJsonFile(std::string_view FileName);

	private:
		static std::string InitializeHeader(size_t numOfEnt);
		static std::string InitializeSignatureJsonFile(size_t numOfEnt);
		static std::string InitializeComponentStr(size_t numOfEnt = 1, bool writingLevel = true);
		static std::string InitializeParentStr(size_t numOfEnt = 1);

	};
}