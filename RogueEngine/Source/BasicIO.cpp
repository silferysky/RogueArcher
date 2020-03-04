/* Start Header ************************************************************************/
/*!
\file           BasicIO.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\brief          This file contains the function definitions for BasicIO

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Precompiled.h"
#include "BasicIO.h"
#include "logger.h"


namespace Rogue
{
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

	void BasicIO::WriteLevelFilesJsonFile(std::string_view FileName)
	{
		std::ostringstream strstream;
		strstream << "{\n  \"Files\": \"Level 1.json;Level 2.json\"";
		strstream << "\n}";
	}

	void BasicIO::WriteLevelJsonFile(std::string_view FileName, size_t numOfEnt)
	{
		std::ostringstream strstream;

		strstream << InitializeHeader(numOfEnt);
		strstream << InitializeComponentStr(numOfEnt);
		strstream << InitializeParentStr(numOfEnt);
		strstream << "\n}";

		WriteFile(FileName.data(), strstream.str().c_str());
	}

	void BasicIO::WriteArchetypeJsonFile(std::string_view FileName)
	{
		std::ostringstream strstream;
		
		strstream << "{\n  \"Signature\": 0,";
		strstream << "\n  \"Entity\": \"\",";
		strstream << "\n  \"Children\": \"\"";

		strstream << "\n}";

		WriteFile(FileName.data(), strstream.str().c_str());
	}

	void BasicIO::WriteArchetypeListJsonFile(std::string_view FileName)
	{
		std::ostringstream strstream;
		strstream << "{\n  \"ArchetypeList\": \"\"\n}";
		WriteFile(FileName.data(), strstream.str().c_str());
	}

	std::string BasicIO::InitializeHeader(size_t numOfEnt)
	{
		std::ostringstream strstream;

		//For EntCount
		strstream << "{\n  \"MaxEntityCount\": " << numOfEnt;
		strstream << ",\n  \"EntityCount\": " << 0;

		//For Camera
		strstream << ",\n  \"CameraMinX\": " << -3392.0f;
		strstream << ",\n  \"CameraMinY\": " << -933.0f;
		strstream << ",\n  \"CameraMaxX\": " << 2388.0f;
		strstream << ",\n  \"CameraMaxY\": " << 1105.0f;
		strstream << ",\n  \"CameraZoom\": " << 1.0;

		strstream << ",\n  \"Tileset\": \"\"";

		strstream << ",\n  \"StartPosX\": " << 0;
		strstream << ",\n  \"StartPosY\": " << 0;

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
			strstream << ",\n  \"Signature" << i << "\": 0";
		}
		return strstream.str();
	}

	std::string BasicIO::InitializeComponentStr(size_t numOfEnt, bool writingLevel)
	{
		std::ostringstream strstream;

		for (size_t i = 0; i < numOfEnt; ++i)
		{
			//Header
			strstream << ",\n  \"Entity" << i << "\": \"";

			/*if (!writingLevel)
			{
				strstream << "Name{Entity" << i << "}|";
				strstream << "Tag{}|";
			}

			//Sprite
			strstream << "Sprite{test.bmp}|";

			//Rigidbody
			//AccelerationX, AccelerationY, VelocityX, VelocityY, InvMass, Volume, isStatic
			strstream << "Rigidbody{0;0;0;0;1;1;0}|";

			//Transform
			//PositionX, PositionY, ScaleX, ScaleY, Rotate
			strstream << "Transform{0;0;1;1;0}|";

			//BoundingCircle
			//Radius
			strstream << "BoundingCircle{0}|";

			//BoxCollider
			//Number of vertices
			strstream << "BoxCollider{4}|";

			//PlayerController
			strstream << "PlayerController{1}|";

			//LogicComponent
			//AI Type, First State, All States available (Can be more than 3 values, and does not need to be ascending)
			strstream << "LogicController{1;3;3}";

			//StatsComponent
			//HP, Speed, Detect Range, Attack Range
			strstream << "StatsComponent{1;1;3;3}";*/

			//End
			strstream << "\"";
		}
		return strstream.str();
	}
	std::string BasicIO::InitializeParentStr(size_t numOfEnt)
	{
		std::ostringstream strstream;
		
		for (size_t i = 0; i < numOfEnt; ++i)
		{
			//Header
			strstream << ",\n  \"EntityParent" << i << "\": " << MAX_ENTITIES;
		}

		return strstream.str();
	}
}