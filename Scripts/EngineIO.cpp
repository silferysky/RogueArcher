#pragma once
#include "EngineIO.h"

EngineIO::EngineIO()
{
	//TODO: Add all initial fileIO reading here
}

EngineIO::~EngineIO()
{
	//TODO: Remove all initial fileIO readings here
	//Also, overwrite save data
}

void EngineIO::ReadFile(FileIOID id)
{
	const char* FileName = GetFileName(id);
	std::ifstream iStream(FileName);

	//To be replaced with actual player data
	int hp, atk, def, count = 0;

	if (iStream.is_open())
	{
		std::string line;

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
			while (std::getline(iStream, line))
			{
				RE_CORE_INFO(line);
			}
			break;
		case PlayerStatsData:
			while (std::getline(iStream, line))
			{
				switch (count)
				{
				case 0:
					hp = std::stoi(line);
					break;
				case 1:
					atk = std::stoi(line);
					break;
				case 2:
					def = std::stoi(line);
					break;
				default:
					break;
				}
				++count;
			}
			RE_INFO(hp);
			RE_INFO(atk);
			RE_INFO(def);
			break;
		default:
			RE_CORE_WARN("FILEIO READ: ID does not match");
			break;
		}

		iStream.close();
	}
	else
	{
		RE_CORE_ERROR("FILEIO READ: Unable to open file");
	}
}

void EngineIO::WriteFile(FileIOID id)
{
	const char* FileName = GetFileName(id);
	std::ofstream oStream(FileName);

	if (oStream.is_open())
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
			oStream << "HI" << std::endl;
			oStream << "First test run here!";
			break;
		case PlayerStatsData:
			oStream << "1" << std::endl;
			oStream << "2" << std::endl;
			oStream << "3" << std::endl;
			break;
		default:
			RE_CORE_WARN("FILEIO WRITE: ID does not match");
			break;
		}

		oStream.close();
	}
	else
	{
		RE_CORE_ERROR("FILEIO WRITE: Unable to open file");
	}
}

const char* EngineIO::GetFileName(FileIOID id)
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
	default: //File IO Test is default
		return "DATA/test.txt";
	}
}