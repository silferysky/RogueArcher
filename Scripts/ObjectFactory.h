#pragma once
#include "Main.h"
#include "RogueEngine.h"
#include "Coordinator.h"
#include "FileIO.h"

class ObjectFactory
{
	RESerialiser m_Serialiser;

public:
	void SaveLevel(const char* fileName, const char* filePath = nullptr);
	void LoadLevel(const char* fileName, const char* filePath = nullptr);

private:
	ComponentType GetTypeName(int index) const;
};