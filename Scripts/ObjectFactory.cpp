#pragma once
#include "ObjectFactory.h"
#include "ComponentList.h"

void ObjectFactory::SaveLevel(const char* fileName, const char* filePath)
{
	int entCount = gEngine.RECoordinator.Size<EntityManager>();
	m_Serialiser.WriteToFile(fileName, "EntityCount", entCount);

	for (uint32_t i = 0; i < entCount; ++i)
	{
		
	}

	RE_INFO("LEVEL SAVED");
}
void ObjectFactory::LoadLevel(const char* fileName, const char* filePath)
{
	rapidjson::Document level = m_Serialiser.DeserialiseFromFile(fileName);
	int entCount;
	float curEntSig;
	entCount = level["EntityCount"].GetInt();

	for (int i = 0; i < entCount; ++i)
	{
		Entity curEnt = gEngine.RECoordinator.CreateEntity();
		curEntSig = level["Sig" + i].GetFloat();
		for (int j = 0; j < MAX_COMPONENTS; ++j)
		{
			if (curEntSig >= 1.0f)
			{
				gEngine.RECoordinator.AddComponent(curEnt, GetTypeName(j));
				--curEntSig;
			}
			curEntSig *= 2;
		}
	}
}

ComponentType ObjectFactory::GetTypeName(int index) const
{
	const char * cmpName;
	switch (index)
	{
	case 0:
		cmpName = "SpriteComponent";
	case 1:
		cmpName = "Rigidbody";
	case 2:
		cmpName = "Transform";
	case 3:
		cmpName = "CircleCollider2D";
	default:
		return 0;
	}

	return gEngine.RECoordinator.GetComponentType(cmpName);
}

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