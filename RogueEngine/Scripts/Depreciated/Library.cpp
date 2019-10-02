#pragma once
#include "Library.h"

Library::Library()
{
}

Library::~Library()
{

}

void Library::IntepretIO(LibraryID id, std::string string)
{
	switch (id)
	{
	case ComponentID:
	case EntityID:
	case SystemID:
		break;
	case ComponentData:
		break;
	case EntityData:
		break;
	case SystemData:
		break;
	case FileIOTest:
		RE_INFO(string);
		break;
	case PlayerStatsData:
	{
		std::stringstream data(string);
		std::string segment;
		std::vector<std::string> segmentList;

		//This part segments each line
		while (std::getline(data, segment, '\f'))
			segmentList.push_back(segment);

		//This section is only for debugging
		segment = std::string();
		int count = 0;
		for (std::string s : segmentList)
		{
			switch (count++)
			{
			case 0:
				segment.append("HP: ");
				break;
			case 1:
				segment.append("ATK: ");
				break;
			case 2:
				segment.append("DEF: ");
				break;
			default:
				RE_CORE_WARN("UNKNOWN COUNT READ AT COUNT {0}", count);
				break;
			}
			segment.append(s);
			segment.append("|");
		}
		PrintData(segment);
		break;
	}
	default:
		RE_CORE_WARN("FAILED TO INTEPRET DATA");
		break;
	}
}

std::string Library::GetFileName(LibraryID id)
{
	switch (id)
	{
	case ComponentID:
		return "DATA/ComponentID.txt";
		break;
	case EntityID:
		return "DATA/EntityID.txt";
		break;
	case SystemID:
		return "DATA/SystemID.txt";
		break;
	case ComponentData:
		return "DATA/ComponentData.txt";
		break;
	case EntityData:
		return "DATA/EntityData.txt";
		break;
	case SystemData:
		return "DATA/SystemData.txt";
		break;
	case PlayerStatsData:
		return "DATA/PlayerStatsData.txt";
		break;
	case FileIOTest:
		return "DATA/test.txt";
		break;
	default:
		return "DATA/ErrorLog.txt";
		break;
	}
}

void Library::PrintData(std::string data)
{
	RE_INFO(data);
}

void Library::IOTest()
{
	LIBRARY_READ_INTEPRET(ComponentData);
	LIBRARY_READ_INTEPRET(EntityData);
	LIBRARY_READ_INTEPRET(SystemData);
	LIBRARY_READ_INTEPRET(PlayerStatsData);

	LIBRARY_WRITE(ComponentData, "HI");
	LIBRARY_WRITE(EntityData, "HI!");
	LIBRARY_WRITE(SystemData, "HI!!");
}