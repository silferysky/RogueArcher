#pragma once
#include "ObjectFactory.h"
#include <bitset>

void ObjectFactory::SaveLevel(const char* fileName)
{
	Entity entCount = m_activeEntities.size();
	EntityManager* em = &gEngine.m_coordinator.GetEntityManager();
	m_Serialiser.WriteToFile(fileName, "EntCount", (int)entCount);
	
	std::vector<Entity>::iterator entIt = m_activeEntities.begin();
	for (int i = 0; i < entCount; ++i)
	{
		std::stringstream varName;
		std::string stdstr;
		const char* cstr;
		int varNum = 0;
		int convertSig = em->GetSignature(*entIt).to_ulong();

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		varName << "Signature" << i;
		stdstr = varName.str();
		cstr = stdstr.c_str();
		m_Serialiser.WriteToFile(fileName, cstr, convertSig);

		//TODO: Change it to more dynamic
		SpriteComponent s = gEngine.m_coordinator.GetComponent<SpriteComponent>(i);
		Rigidbody r = gEngine.m_coordinator.GetComponent<Rigidbody>(i);
		Transform t = gEngine.m_coordinator.GetComponent<Transform>(i);
		CircleCollider2D c = gEngine.m_coordinator.GetComponent<CircleCollider2D>(i);

		//Copypasta for each new variable//
		//SpriteComponent does not need to save values, so just save signature
		/*varNum = 0;
		CLEARNSETSTR(varName, i, "sc", varNum);
		m_Serialiser.WriteToFile(fileName, cstr, s.getTexture());
    */
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

		++entIt;
	}

	RE_INFO("LEVEL SAVED");
}

void ObjectFactory::LoadLevel(const char* fileName)
{
	rapidjson::Document level = m_Serialiser.DeserialiseFromFile(fileName);
	Signature curEntSig;
	int entCount = level["EntCount"].GetInt();
	std::stringstream debugStr;

	for (int i = 0; i < entCount; ++i)
	{
		std::stringstream strstream;
		std::string stdstr;
		const char* cstr;
		Entity curEnt = gEngine.m_coordinator.CreateEntity();

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		strstream << "Signature" << i;

		stdstr = strstream.str();
		cstr = stdstr.c_str();
		curEntSig = level[cstr].GetInt();

		//Copypasta for each new variable//
		if (curEntSig.test(0))
		{
			//SpriteComponent does not need to load values, so ignore
			//Still need to -- though, since signature does contain it
			SpriteComponent s;
			/*CLEARNSETSTR(strstream, i, "sc", 0);
			s.m_shader = (unsigned int)level[cstr].GetInt();
			CLEARNSETSTR(strstream, i, "sc", 1);
			s.m_VAO = (unsigned int)level[cstr].GetInt();
			CLEARNSETSTR(strstream, i, "sc", 2);
			s.m_VBO = (unsigned int)level[cstr].GetInt();
			CLEARNSETSTR(strstream, i, "sc", 3);
			s.m_EBO = (unsigned int)level[cstr].GetInt();*/

			gEngine.m_coordinator.AddComponent(curEnt, s);
		}
		///////////////////////////////////

		//Copypasta for each new variable//
		if (curEntSig.test(1))
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

			gEngine.m_coordinator.AddComponent(curEnt, r);
		}
		///////////////////////////////////
		
		//Copypasta for each new variable//
		if (curEntSig.test(2))
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

			gEngine.m_coordinator.AddComponent(curEnt, t);
		}
		///////////////////////////////////

		//Copypasta for each new variable//
		if (curEntSig.test(3))
		{
			CircleCollider2D cc;
			CLEARNSETSTR(strstream, i, "ccc", 0);
			cc.setRadius(level[cstr].GetFloat());

			gEngine.m_coordinator.AddComponent(curEnt, cc);
		}
		///////////////////////////////////


		//ADD NEW COMPONENT LOADING HERE, BASED ON BITMAP
		//Finally add Entity reference to entity vector
		m_activeEntities.push_back(curEnt);

		debugStr.clear();
		debugStr.str("");
		debugStr << "Entity " << i << "'s Signature: " << gEngine.m_coordinator.GetEntityManager().GetSignature(i).to_ulong();
		RE_INFO(debugStr.str());
	}
	RE_INFO("LEVEL LOADED");
	std::stringstream infoStr;
	infoStr << entCount << " ENTITIES LOADED";
	RE_INFO(infoStr.str());

}

std::vector<Entity> ObjectFactory::GetActiveEntity() const
{
	return m_activeEntities;
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

	return gEngine.m_coordinator.GetComponentType(cmpName);
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