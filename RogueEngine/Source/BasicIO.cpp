#pragma once
#include <fstream>
#include "BasicIO.h"
#include "logger.h"

#define BUFFER_SIZE 10000

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

void BasicIO::WriteJsonFile(std::string FileName, size_t numOfEnt)
{
	char writeStr[BUFFER_SIZE] = { 0 };

	//For EntCount
	strcat_s(writeStr, "{\"EntityCount\":");
	strcat_s(writeStr, std::to_string(numOfEnt).c_str());

	//For Background Texture
	strcat_s(writeStr, ",\"BackgroundTexture\": \"\"");

	//For Signature
	for (size_t i = 0; i < numOfEnt; ++i)
	{
		strcat_s(writeStr, ",\"Signature");
		strcat_s(writeStr, std::to_string(i).c_str());
		strcat_s(writeStr, "\":0");
	}

	//For each component
	for (size_t i = 0; i < numOfEnt; ++i)
	{
		//For SpriteComponent
		strcat_s(writeStr, ",\"Entity");
		strcat_s(writeStr, std::to_string(i).c_str());
		strcat_s(writeStr, "Sprite0\":\"\"");
		//Nothing

		//For RigidbodyComponent
		for (size_t j = 0; j < 6; ++j)
		{
			strcat_s(writeStr, ",\"Entity");
			strcat_s(writeStr, std::to_string(i).c_str());
			strcat_s(writeStr, "Rigidbody");
			strcat_s(writeStr, std::to_string(j).c_str());
			if (j == 4 || j == 5)
				strcat_s(writeStr, "\":1.0");
			else
				strcat_s(writeStr, "\":0.0");
		}

		//For TransformComponent
		for (size_t j = 0; j < 5; ++j)
		{
			strcat_s(writeStr, ",\"Entity");
			strcat_s(writeStr, std::to_string(i).c_str());
			strcat_s(writeStr, "Transform");
			strcat_s(writeStr, std::to_string(j).c_str());
			if (j == 2 || j == 3)
				strcat_s(writeStr, "\":0.5");
			else
				strcat_s(writeStr, "\":0.0");
		}

		//For CircleCollider
		strcat_s(writeStr, ",\"Entity");
		strcat_s(writeStr, std::to_string(i).c_str());
		strcat_s(writeStr, "CircleCollider0\":0.0");

		//For Box Collider
		strcat_s(writeStr, ",\"Entity");
		strcat_s(writeStr, std::to_string(i).c_str());
		strcat_s(writeStr, "BoxCollider0\":0");
		for (size_t j = 1; j < 9; ++j)
		{
			strcat_s(writeStr, ",\"Entity");
			strcat_s(writeStr, std::to_string(i).c_str());
			strcat_s(writeStr, "BoxCollider");
			strcat_s(writeStr, std::to_string(j).c_str());
			strcat_s(writeStr, "\":0.0");
		}
		strcat_s(writeStr, "\n");
	}
	strcat_s(writeStr, "}");

	WriteFile(FileName, writeStr);
}