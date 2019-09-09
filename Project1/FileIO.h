#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/writer.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/ostreamwrapper.h>
#include <iostream>
#include <fstream>
#include <cstdlib>


	/*bool ChangeData(const char* FileName, const char* DataTitle, const char (&DataToChange)[])
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

		//std::cout << jsonStr << '\n';

		doc[DataTitle] = DataToChange;
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
*/