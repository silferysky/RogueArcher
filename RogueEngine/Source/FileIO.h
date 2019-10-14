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

class RESerialiser
{
public:
	template <typename T>
	static bool WriteToFile(const char* FileName, const char* DataTitle, T DataToChange)
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
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer{ buffer };
		doc.Accept(writer);

		if (doc.HasParseError())
		{
			std::cout << "Error  : " << doc.GetParseError() << '\n'
				<< "Offset : " << doc.GetErrorOffset() << '\n';
			return EXIT_FAILURE;
		}

		const std::string jsonStr{ buffer.GetString() };

		//std::cout << jsonStr << '\n';

		std::ostringstream ss;
		ss << DataToChange;
		//if (typeid(DataToChange) == std::string)
		//{
		//	doc[DataTitle].SetString(rapidjson::GenericStringRef<char>(ss.str().c_str()), doc.GetAllocator()); // = DataToChange;
		//}
		if (std::is_same<T, int>::value)
		{
			doc[DataTitle].SetInt((int)DataToChange);
		}
		else if (std::is_same<T, float>::value)
		{
			doc[DataTitle].SetFloat((float)DataToChange);
		}
		else
		{
			doc[DataTitle].SetString(rapidjson::GenericStringRef<char>(ss.str().c_str()), doc.GetAllocator()); // = DataToChange;
		}

		std::ofstream ofs{ FileName };
		if (!ofs.is_open())
		{
			std::cerr << "Could not open file for writing!\n";
			return EXIT_FAILURE;
		}

		rapidjson::OStreamWrapper osw{ ofs };
		rapidjson::PrettyWriter<rapidjson::OStreamWrapper> writer2{ osw };
		doc.Accept(writer2);

		return EXIT_SUCCESS;
	}
	static rapidjson::Document DeserialiseFromFile(const char* FileName);
	static bool ReadFromFile(const char* FileName);
	static bool IfFileExists(const char* FileName);
};
/* test for joel in case he forget/ put in main.cpp
	RESerialiser Serialiser;
	rapidjson::Document haha = Serialiser.DeserialiseFromFile("Resources/PhysicsData.json");
	int i;
	i = haha["Id"].GetInt();
	if(i == 1)
	{
		std::cout << i << std::endl;
	}
	Serialiser.WriteToFile("Resources/test.json", "angle z", 643.54f);
	rapidjson::Document testing = Serialiser.DeserialiseFromFile("Resources/test.json");
	float z;
	z = testing["angle z"].GetFloat();

	std::cout << z << std::endl;*/