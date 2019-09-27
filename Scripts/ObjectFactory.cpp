#pragma once
#include "ObjectFactory.h"

void ObjectFactory::SaveLevel(const char* fileName)
{
	Entity entCount = (Entity)gEngine.RECoordinator.Size("Entity");
	EntityManager* em = &gEngine.RECoordinator.GetEntityManager();
	m_Serialiser.WriteToFile(fileName, "EntCount", (int)entCount);

	for (Entity i = 0; i < entCount; ++i)
	{
		std::stringstream varName;
		std::string stdstr;
		const char* cstr;
		int varNum = 0;
		int convertSig = em->GetSignature(i).to_ulong();

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		varName << "Signature" << i;
		stdstr = varName.str();
		cstr = stdstr.c_str();
		m_Serialiser.WriteToFile(fileName, cstr, convertSig);

		//TODO: Change it to more dynamic
		SpriteComponent s = gEngine.RECoordinator.GetComponent<SpriteComponent>(i);
		Rigidbody r = gEngine.RECoordinator.GetComponent<Rigidbody>(i);
		Transform t = gEngine.RECoordinator.GetComponent<Transform>(i);
		CircleCollider2D c = gEngine.RECoordinator.GetComponent<CircleCollider2D>(i);

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (int)s.shader);
		++varNum;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (int)s.VAO);
		++varNum;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (int)s.VBO);
		++varNum;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (int)s.EBO);
		///////////////////////////////////

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (float)r.getAcceleration().x);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (float)r.getAcceleration().y);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (float)r.getVelocity().x);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, (float)r.getVelocity().y);
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, r.getInvMass());
		++varNum;
		CLEARNSETSTR(varName, i, "rbc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, r.getVolume());
		///////////////////////////////////

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, t.getPosition().x);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, t.getPosition().y);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, t.getScale().x);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, t.getScale().y);
		++varNum;
		CLEARNSETSTR(varName, i, "tc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, t.getRotation());
		///////////////////////////////////

		//Copypasta for each new variable//
		varNum = 0;
		CLEARNSETSTR(varName, i, "ccc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, c.getRadius());
		///////////////////////////////////
	}

	RE_INFO("LEVEL SAVED");
}
void ObjectFactory::LoadLevel(const char* fileName)
{
	rapidjson::Document level = m_Serialiser.DeserialiseFromFile(fileName);
	int curEntSig;
	int entCount = level["EntCount"].GetInt();

	for (int i = 0; i < entCount; ++i)
	{
		std::stringstream strstream;
		std::string stdstr;
		const char* cstr;
		Entity curEnt = gEngine.RECoordinator.CreateEntity();

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		strstream << "Signature" << i;
		stdstr = strstream.str();
		cstr = stdstr.c_str();
		curEntSig = level[cstr].GetInt();

		//Copypasta for each new variable//
		if (curEntSig % 2 == 1)
		{
			SpriteComponent s;
			CLEARNSETSTR(strstream, i, "sc", 0);
			s.shader = (unsigned int)level[cstr].GetInt();
			CLEARNSETSTR(strstream, i, "sc", 1);
			s.VAO = (unsigned int)level[cstr].GetInt();
			CLEARNSETSTR(strstream, i, "sc", 2);
			s.VBO = (unsigned int)level[cstr].GetInt();
			CLEARNSETSTR(strstream, i, "sc", 3);
			s.EBO = (unsigned int)level[cstr].GetInt();

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
			CLEARNSETSTR(strstream, i, "rbc", 0);
			x = level[cstr].GetFloat();
			CLEARNSETSTR(strstream, i, "rbc", 1);
			y = level[cstr].GetFloat();
			r.setAcceleration(Vec2(x, y));
			CLEARNSETSTR(strstream, i, "rbc", 2);
			x = level[cstr].GetFloat();
			CLEARNSETSTR(strstream, i, "rbc", 3);
			y = level[cstr].GetFloat();
			r.setVelocity(Vec2(x, y));
			//CLEARNSETSTR(strstream, i, "rbc", 4);
			//x = level[cstr].GetInt();
			//CLEARNSETSTR(strstream, i, "rbc", 5);
			//y = level[cstr].GetInt();
			//r.getAccForce(Vec2(x, y));
			CLEARNSETSTR(strstream, i, "rbc", 4);
			r.setMass(level[cstr].GetFloat());
			CLEARNSETSTR(strstream, i, "rbc", 5);
			r.setVolume(level[cstr].GetFloat());

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
			CLEARNSETSTR(strstream, i, "tc", 0);
			x = level[cstr].GetFloat();
			CLEARNSETSTR(strstream, i, "tc", 1);
			y = level[cstr].GetFloat();
			t.setPosition(Vec2(x, y));
			CLEARNSETSTR(strstream, i, "tc", 2);
			x = level[cstr].GetFloat();
			CLEARNSETSTR(strstream, i, "tc", 3);
			y = level[cstr].GetFloat();
			t.setScale(Vec2(x, y));
			t.setRotation(level[cstr].GetFloat());

			gEngine.RECoordinator.AddComponent(curEnt, t);
			--curEntSig;
		}
		curEntSig /= 2;
		///////////////////////////////////

		//Copypasta for each new variable//
		if (curEntSig % 2 == 1)
		{
			CircleCollider2D cc;
			CLEARNSETSTR(strstream, i, "ccc", 0);
			cc.setRadius(level[cstr].GetFloat());

			gEngine.RECoordinator.AddComponent(curEnt, cc);
			--curEntSig;
		}
		curEntSig /= 2;
		///////////////////////////////////


		//ADD NEW COMPONENT LOADING HERE, BASED ON BITMAP


	}
	RE_INFO("LEVEL LOADED");
	std::stringstream infoStr;
	infoStr << entCount << " ENTITIES LOADED";
	RE_INFO(infoStr.str());

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