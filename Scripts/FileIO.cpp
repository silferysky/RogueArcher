#include "FileIO.h"

bool WriteToFile(std::string FileName, const char* DataTitle, std::string DataToChange)
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

		doc[DataTitle].SetString(rapidjson::GenericStringRef<char>(DataToChange.c_str()));// = DataToChange;
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

bool WriteToFilePretty(const char* FileName, const char* DataTitle, const char* DataToChange)
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

		doc[DataTitle].SetString(rapidjson::GenericStringRef<char>(DataToChange));// = DataToChange;
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
}

bool ReadFromFile(const char* FileName)
{
	FILE* pFile = nullptr;
	fopen_s(&pFile,FileName, "r");
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
