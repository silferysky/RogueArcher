#pragma once
#include "ObjectFactory.h"
#include "ComponentList.h"

void ObjectFactory::SaveLevel(const char* fileName)
{
	Entity entCount = gEngine.RECoordinator.Size("Entity");
	EntityManager* em = &gEngine.RECoordinator.GetEntityManager();
	m_Serialiser.WriteToFile(fileName, "EntCount", entCount);

	for (Entity i = 0; i < entCount; ++i)
	{
		std::stringstream varName;
		varName << "Signature" << i;
		int convertSig = em->GetSignature(i).to_ulong();
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), convertSig);

		//TODO: Change it to more dynamic
		//SpriteComponent s = gEngine.RECoordinator.GetComponent<SpriteComponent>(i);
		//Rigidbody r = gEngine.RECoordinator.GetComponent<Rigidbody>(i);
		//Transform t = gEngine.RECoordinator.GetComponent<Transform>(i);
		//CircleCollider2D c = gEngine.RECoordinator.GetComponent<CircleCollider2D>(i);

		//varName.clear();
		//varName << "e" << i;
		//RE_INFO(varName);
	}

	RE_INFO("LEVEL SAVED");
}
void ObjectFactory::LoadLevel(const char* fileName)
{
	rapidjson::Document level = m_Serialiser.DeserialiseFromFile(fileName);
	int entCount;
	float curEntSig;
	std::stringstream sig;
	entCount = level["EntityCount"].GetInt();

	for (int i = 0; i < entCount; ++i)
	{
		Entity curEnt = gEngine.RECoordinator.CreateEntity();
		sig << "Sig" << i;
		curEntSig = level[sig.str().c_str()].GetFloat();
		for (int j = 0; j < MAX_COMPONENTS; ++j)
		{
			if (curEntSig >= 1.0f)
			{
				//gEngine.RECoordinator.AddComponent(curEnt, GetTypeName(j));
				--curEntSig;
			}
			curEntSig *= 2;
		}
	}
}

ComponentType ObjectFactory::GetCmpType(int index) const
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