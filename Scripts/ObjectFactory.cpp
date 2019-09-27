#pragma once
#include "ObjectFactory.h"

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
		int varNum = 0;

		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), convertSig);

		//TODO: Change it to more dynamic
		SpriteComponent s = gEngine.RECoordinator.GetComponent<SpriteComponent>(i);
		Rigidbody r = gEngine.RECoordinator.GetComponent<Rigidbody>(i);
		Transform t = gEngine.RECoordinator.GetComponent<Transform>(i);
		CircleCollider2D c = gEngine.RECoordinator.GetComponent<CircleCollider2D>(i);

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), s.shader);
		++varNum;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), s.VAO);
		++varNum;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), s.VBO);
		++varNum;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), s.EBO);
		///////////////////////////////////

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), (float)r.getAcceleration().x);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), (float)r.getAcceleration().y);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), (float)r.getVelocity().x);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), (float)r.getVelocity().y);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), r.getInvMass());
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), r.getVolume());
		///////////////////////////////////

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), t.getPosition().x);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), t.getPosition().y);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), t.getScale().x);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), t.getScale().y);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), t.getRotation());
		///////////////////////////////////

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "ccc", varNum);
		m_Serialiser.WriteToFile(fileName, varName.str().c_str(), c.getRadius());
		///////////////////////////////////
	}

	RE_INFO("LEVEL SAVED");
}
void ObjectFactory::LoadLevel(const char* fileName)
{
	rapidjson::Document level = m_Serialiser.DeserialiseFromFile(fileName);
	int entCount;
	int curEntSig;
	entCount = level["EntCount"].GetInt();

	for (int i = 0; i < entCount; ++i)
	{
		std::stringstream str;
		Entity curEnt = gEngine.RECoordinator.CreateEntity();
		str << "Signature" << i;
		curEntSig = level[str.str().c_str()].GetInt();

		//Copypasta for each new variable//
		if (curEntSig % 2 == 1)
		{
			SpriteComponent s;
			CLEARNSETSTR(str, i, "sc", 0);
			s.shader = (unsigned int)level[str.str().c_str()].GetInt();
			CLEARNSETSTR(str, i, "sc", 1);
			s.VAO = (unsigned int)level[str.str().c_str()].GetInt();
			CLEARNSETSTR(str, i, "sc", 2);
			s.VBO = (unsigned int)level[str.str().c_str()].GetInt();
			CLEARNSETSTR(str, i, "sc", 3);
			s.EBO = (unsigned int)level[str.str().c_str()].GetInt();

			gEngine.RECoordinator.AddComponent(curEnt, s);
			--curEntSig;
		}
		curEntSig /= 2;
		///////////////////////////////////

		//Copypasta for each new variable//
		if (curEntSig % 2 == 1)
		{
			Rigidbody r;
			float x, y;
			CLEARNSETSTR(str, i, "rbc", 0);
			x = level[str.str().c_str()].GetFloat();
			CLEARNSETSTR(str, i, "rbc", 1);
			y = level[str.str().c_str()].GetFloat();
			r.setAcceleration(Vec2(x, y));
			CLEARNSETSTR(str, i, "rbc", 2);
			x = level[str.str().c_str()].GetFloat();
			CLEARNSETSTR(str, i, "rbc", 3);
			y = level[str.str().c_str()].GetFloat();
			r.setVelocity(Vec2(x, y));
			//CLEARNSETSTR(str, i, "rbc", 4);
			//x = level[str.str().c_str()].GetInt();
			//CLEARNSETSTR(str, i, "rbc", 5);
			//y = level[str.str().c_str()].GetInt();
			//r.getAccForce(Vec2(x, y));
			CLEARNSETSTR(str, i, "rbc", 4);
			r.setMass(level[str.str().c_str()].GetInt());
			CLEARNSETSTR(str, i, "rbc", 5);
			r.setVolume(level[str.str().c_str()].GetInt());

			gEngine.RECoordinator.AddComponent(curEnt, r);
			--curEntSig;
		}
		curEntSig /= 2;
		///////////////////////////////////
		
		//Copypasta for each new variable//
		if (curEntSig % 2 == 1)
		{
			Transform t;
			float x, y;
			CLEARNSETSTR(str, i, "tc", 0);
			x = level[str.str().c_str()].GetFloat();
			CLEARNSETSTR(str, i, "tc", 1);
			y = level[str.str().c_str()].GetFloat();
			t.setPosition(Vec2(x, y));
			CLEARNSETSTR(str, i, "tc", 2);
			x = level[str.str().c_str()].GetFloat();
			CLEARNSETSTR(str, i, "tc", 3);
			y = level[str.str().c_str()].GetFloat();
			t.setScale(Vec2(x, y));
			t.setRotation(level[str.str().c_str()].GetFloat());

			gEngine.RECoordinator.AddComponent(curEnt, t);
			--curEntSig;
		}
		curEntSig /= 2;
		///////////////////////////////////

		//Copypasta for each new variable//
		if (curEntSig % 2 == 1)
		{
			CircleCollider2D cc;
			CLEARNSETSTR(str, i, "ccc", 0);
			cc.setRadius(level[str.str().c_str()].GetFloat());

			gEngine.RECoordinator.AddComponent(curEnt, t);
			--curEntSig;
		}
		curEntSig /= 2;
		///////////////////////////////////
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