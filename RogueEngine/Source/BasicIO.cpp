#pragma once
#include <fstream>
#include "BasicIO.h"
#include "logger.h"
#include <sstream>

std::string BasicIO::ReadFile(std::string FileName)
{
	std::string toReturnStr = std::string();
	std::ifstream iStream(FileName);

	if (iStream.is_open())
	{
		std::string line;

		while (std::getline(iStream, line))
		{
			toReturnStr.append(line);
			toReturnStr.append("\n");
		}
	}
	else
	{
		RE_CORE_ERROR("FILEIO READ: Unable to open file");
	}

	iStream.close();
	return toReturnStr;
}

void BasicIO::WriteFile(std::string FileName, std::string strToWrite)
{
	std::ofstream oStream(FileName);

	if (oStream.is_open())
	{
		oStream << strToWrite;
	}
	else
	{
		RE_CORE_ERROR("FILEIO WRITE: Unable to open file");
	}
	oStream.close();
}

void BasicIO::WriteLevelJsonFile(std::string FileName, size_t numOfEnt)
{
	std::ostringstream strstream;

	strstream << InitializeHeader(numOfEnt, true);
	strstream << InitializeComponentStr(numOfEnt);
	strstream << "\n}";

	WriteFile(FileName, strstream.str().c_str());
}

void BasicIO::WriteArchetypeJsonFile(std::string FileName, size_t numOfEnt)
{
	std::ostringstream strstream;

	strstream << InitializeHeader(numOfEnt);
	strstream << InitializeComponentStr(numOfEnt);
	strstream << "\n}";

	WriteFile(FileName, strstream.str().c_str());
}

std::string BasicIO::InitializeHeader(size_t numOfEnt, bool includeBackground)
{
	std::ostringstream strstream;

	//For EntCount
	strstream << "{\n    \"EntityCount\": " << numOfEnt;

	//For Background Texture
	if (includeBackground)
		strstream << ",\n    \"BackgroundTexture\": \"test.bmp\"";

	//For Signature
	strstream << InitializeSignatureJsonFile(numOfEnt);

	return strstream.str();
}

std::string BasicIO::InitializeSignatureJsonFile(size_t numOfEnt)
{
	std::ostringstream strstream;

	//For Signature
	for (size_t i = 0; i < numOfEnt; ++i)
	{
		strstream << ",\n    \"Signature" << i << "\": 0";
	}
	return strstream.str();
}

std::string BasicIO::InitializeComponentStr(size_t numOfEnt)
{
	std::ostringstream strstream;

	for (size_t i = 0; i < numOfEnt;)
	{
		//Header
		strstream << ",\n    \"Entity" << i << "\": \"";

		//Sprite
		strstream << "Sprite{test.bmp}|";

		//Rigidbody
		//AccelerationX, AccelerationY, VelocityX, VelocityY, InvMass, Volume, isStatic
		strstream << "Rigidbody{0;0;0;0;1;1;0}|";

		//Transform
		//PositionX, PositionY, ScaleX, ScaleY, Rotate
		strstream << "Transform{0;0;1;1;0}|";

		//CircleCollider
		//Radius
		strstream << "CircleCollider{0}|";

		//BoxCollider
		//Number of vertices
		strstream << "BoxCollider{4}|";

		//PlayerController
		strstream << "PlayerController{}|";

		//LogicComponent
		//AI Type, First State, All States available (Can be more than 3 values, and does not need to be ascending)
		strstream << "LogicController{1;3;3}";

		//End
		strstream << "\"";

		if (++i < numOfEnt)
			strstream << ",";
	}
	return strstream.str();
}