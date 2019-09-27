#pragma once
#include "Main.h"
#include "RogueEngine.h"
#include "Coordinator.h"
#include "FileIO.h"

class ObjectFactory
{
	RESerialiser m_Serialiser;

public:
	void SaveLevel(const char* fileName);
	void LoadLevel(const char* fileName);

private:
	ComponentType GetCmpType(int index) const;
};