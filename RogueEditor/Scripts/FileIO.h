#pragma once
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <sstream>

template <typename T>
bool WriteToFile(const char* FileName, const char* DataTitle, T DataToChange)
{
	{
		std::ifstream ifs{ FileName };
		if (!ifs.is_open())
		{
			std::cerr << "Could not open file for reading!\n";
			return EXIT_FAILURE;
		}

		rapidjson::IStreamWrapper Wrapper{ ifs };

		rapidjson::Document doc{};
		doc.ParseStream(Wrapper);

		rapidjson::StringBuffer buffer{};
		rapidjson::Writer<rapidjson::StringBuffer> writer{ buffer };
		doc.Accept(writer);

		if (doc.HasParseError())
		{
			std::cout << "Error  : " << doc.GetParseError() << '\n'
				<< "Offset : " << doc.GetErrorOffset() << '\n';
			return EXIT_FAILURE;
		}

		const std::string jsonStr{ buffer.GetString() };

		std::cout << jsonStr << '\n';

		std::ostringstream ss;
		ss << DataToChange;
		doc[DataTitle].SetString(rapidjson::GenericStringRef<char>(ss.str().c_str()), doc.GetAllocator()); // = DataToChange;
		std::ofstream ofs{ FileName };
		if (!ofs.is_open())
		{
			std::cerr << "Could not open file for writing!\n";
			return EXIT_FAILURE;
		}

		rapidjson::OStreamWrapper osw{ ofs };
		rapidjson::Writer<rapidjson::OStreamWrapper> writer2{ osw };
		doc.Accept(writer2);

		return EXIT_SUCCESS;
	}
}

bool ReadFromFile(const char* FileName);
bool IfFileExists(const char* FileName);