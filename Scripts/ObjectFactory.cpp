#pragma once
#include "ObjectFactory.h"
#include <bitset>
#include <random>
#define RAND_LARGE 500
#define RAND_SMALL 5

void ObjectFactory::LoadLevel(const char* fileName)
{
	rapidjson::Document level = m_Serialiser.DeserialiseFromFile(fileName);

	Signature currentSignature;
	std::stringstream debugStr;

	//For Entity Count
	Entity entCount = level["EntCount"].GetInt();

	//For Background
	Entity backgroundEnt = gEngine.m_coordinator.CreateEntity();
	std::string backgroundStr = level["BackgroundTexture"].GetString();

	SpriteComponent backgroundSprite = SpriteComponent();
	backgroundSprite.setTexture(backgroundStr.c_str());

	TransformComponent backgroundTransform = TransformComponent();
	backgroundTransform.setPosition(Vec2(-0.5f, 0.0f));
	backgroundTransform.setScale(Vec2(1.0f, 1.0f));
	backgroundTransform.setRotation(0.0f);

	RigidbodyComponent backgroundRigidbody = RigidbodyComponent();

	gEngine.m_coordinator.AddComponent(backgroundEnt, backgroundSprite);
	gEngine.m_coordinator.AddComponent(backgroundEnt, backgroundTransform);
	gEngine.m_coordinator.AddComponent(backgroundEnt, backgroundRigidbody);
	m_activeEntities.push_back(backgroundEnt);

	for (Entity entity = 0; entity < entCount; ++entity)
	{
		std::stringstream strstream;
		std::string stdstr;
		const char* cstr;
		Entity curEnt = gEngine.m_coordinator.CreateEntity();

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		strstream << "Signature" << entity;

		stdstr = strstream.str();
		cstr = stdstr.c_str();
		currentSignature = level[cstr].GetInt();

		//Copypasta for each new variable//
		if (currentSignature.test(static_cast<int>(SPRITE)))
		{
			SpriteComponent s{};
			const char* path;
			CLEARNSETSTR(strstream, entity, "sc", 0);
			path = level[cstr].GetString();
			//s.setTexture(path);
			s.setTexture("test.bmp");
			
			gEngine.m_coordinator.AddComponent(curEnt, s);
		}
		///////////////////////////////////

		//Copypasta for each new variable//
		if (currentSignature.test(static_cast<int>(RIGIDBODY)))
		{
			RigidbodyComponent r{};
			float x, y;
			CLEARNSETSTR(strstream, entity, "rbc", 0);
			x = level[cstr].GetFloat();
			CLEARNSETSTR(strstream, entity, "rbc", 1);
			y = level[cstr].GetFloat();

			if (x != 0.0f && y != 0.0f)
			{
				std::cout << "HI" << std::endl;
				r.setAcceleration(Vec2(x, y));
			}
			CLEARNSETSTR(strstream, entity, "rbc", 2);
			x = level[cstr].GetFloat();
			CLEARNSETSTR(strstream, entity, "rbc", 3);
			y = level[cstr].GetFloat();
			r.setVelocity(Vec2(x, y));
			//CLEARNSETSTR(strstream, entity, "rbc", 4);
			//x = level[cstr].GetInt();
			//CLEARNSETSTR(strstream, entity, "rbc", 5);
			//y = level[cstr].GetInt();
			//r.getAccForce(Vec2(x, y));
			CLEARNSETSTR(strstream, entity, "rbc", 4);
			r.setMass(level[cstr].GetFloat());
			CLEARNSETSTR(strstream, entity, "rbc", 5);
			r.setVolume(level[cstr].GetFloat());

			gEngine.m_coordinator.AddComponent(curEnt, r);
		}
		///////////////////////////////////

		//Copypasta for each new variable//
		if (currentSignature.test(static_cast<int>(TRANSFORM)))
		{
			TransformComponent t{};
			float x, y;
			//CLEARNSETSTR(strstream, entity, "tc", 0);
			//x = level[cstr].GetFloat();
			//CLEARNSETSTR(strstream, entity, "tc", 1);
			//y = level[cstr].GetFloat();
			//t.setPosition(Vec2(x, y));
			//CLEARNSETSTR(strstream, entity, "tc", 2);
			//x = level[cstr].GetFloat();
			//CLEARNSETSTR(strstream, entity, "tc", 3);
			//y = level[cstr].GetFloat();
			//t.setScale(Vec2(x, y));
			//t.setRotation(level[cstr].GetFloat());

			t.setPosition(Vec2(1.0f, 0.0f));
			t.setScale(Vec2(0.5f, 0.5f));
			t.setRotation(0.0f);

			gEngine.m_coordinator.AddComponent(curEnt, t);
		}
		///////////////////////////////////

		//Copypasta for each new variable//
		if (currentSignature.test(static_cast<int>(CIRCLECOLLIDER2D)))
		{
			CircleCollider2DComponent cc{};
			CLEARNSETSTR(strstream, entity, "ccc", 0);
			cc.setRadius(level[cstr].GetFloat());

			gEngine.m_coordinator.AddComponent(curEnt, cc);
		}

		///////////////////////////////////

		//Copypasta for each new variable//
		if (currentSignature.test(static_cast<int>(BOXCOLLIDER2D)))
		{
			std::vector<Vec2> vecVec;
			float x, y;
			size_t size, varNum = 0;
			CLEARNSETSTR(strstream, entity, "bcc", varNum);
			size = (size_t)level[cstr].GetInt();
			++varNum;

			for (size_t vertCount = 0; vertCount < size; ++vertCount)
			{
				CLEARNSETSTR(strstream, entity, "bcc", varNum);
				x = level[cstr].GetFloat();
				++varNum;
				CLEARNSETSTR(strstream, entity, "bcc", varNum);
				y = level[cstr].GetFloat();
				++varNum;
				vecVec.push_back(Vec2(x, y));
			}

			BoxCollider2DComponent bc{};
			bc.m_obb = OBB(vecVec);
			bc.m_obb.setSize(size);
			gEngine.m_coordinator.AddComponent(curEnt, bc);
		}
		///////////////////////////////////

		//ADD NEW COMPONENT LOADING HERE, BASED ON BITMAP
		//Finally add Entity reference to entity vector
		m_activeEntities.push_back(curEnt);

		debugStr.clear();
		debugStr.str("");
		debugStr << "Entity " << entity << "'s Signature: " << gEngine.m_coordinator.GetEntityManager().GetSignature(entity).to_ulong();
		RE_INFO(debugStr.str());
	}
	RE_INFO("LEVEL LOADED");
	std::stringstream infoStr;
	infoStr << entCount << " ENTITIES LOADED";
	RE_INFO(infoStr.str());

}

void ObjectFactory::SaveLevel(const char* fileName)
{
	//Minus one off due to Background being part of the list as well.
	//Background is unique and will not be counted to the entCount
	Entity entCount = static_cast<Entity>(m_activeEntities.size() - 1);
	EntityManager* em = &gEngine.m_coordinator.GetEntityManager();
	m_Serialiser.WriteToFile(fileName, "EntCount", (int)entCount);

	for (Entity entity : m_activeEntities)
	{
		std::stringstream varName;
		std::string stdstr;
		const char* cstr;
		int varNum = 0;
		Signature entitySignature = em->GetSignature(entity);

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		varName << "Signature" << entity;
		stdstr = varName.str();
		cstr = stdstr.c_str();
		m_Serialiser.WriteToFile(fileName, cstr, static_cast<int>(entitySignature.to_ulong()));

		//Copypasta for each new variable//
		//SpriteComponent does not need to save values, so just save signature
		///////////////////////////////////

		if (gEngine.m_coordinator.CheckIfComponentExists<SpriteComponent>(entity))
		{
			/*SpriteComponent& s = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

			varNum = 0;
			CLEARNSETSTR(varName, entity, "sc", varNum);
			std::map<const char*, GLuint> textureMap = gEngine.m_coordinator.GetTextureManager().getTextureMap();
			const char* filePath = "";
			
			for (std::map<const char*, GLuint>::iterator it = textureMap.begin();
				it != textureMap.end(); ++it)
			{
				if (it->second == s.getTexture())
				{
					filePath = it->first;
					break;
				}
			}

			std::string filePathStr = filePath;
			m_Serialiser.WriteToFile(fileName, cstr, filePathStr);*/
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<RigidbodyComponent>(entity))
		{
			RigidbodyComponent& r = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(entity);

			varNum = 0;
			CLEARNSETSTR(varName, entity, "rbc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getAcceleration().x);
			++varNum;
			CLEARNSETSTR(varName, entity, "rbc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getAcceleration().y);
			++varNum;
			CLEARNSETSTR(varName, entity, "rbc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getVelocity().x);
			++varNum;
			CLEARNSETSTR(varName, entity, "rbc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getVelocity().y);
			++varNum;
			CLEARNSETSTR(varName, entity, "rbc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getInvMass());
			++varNum;
			CLEARNSETSTR(varName, entity, "rbc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getVolume());
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<TransformComponent>(entity))
		{
			TransformComponent& t = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);

			std::random_device rd;
			std::mt19937 gen(rd());		//For random seed
			std::uniform_int_distribution<> distribution_L(-RAND_LARGE, RAND_LARGE);
			//std::uniform_int_distribution<> distribution_S(-RAND_SMALL, RAND_SMALL);

			//Position, Scale and Rotation
			//Scale is set to 2 decimal places by getting a random int, cast as float,
			//divided by RAND_SMALL, rounded off, then divided by RAND_LARGE and multiply
			//by RAND_SMALL to offset original RAND_SMALL
			//This will give a number with 2 decimal places
			varNum = 0;
			CLEARNSETSTR(varName, entity, "tc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, distribution_L(gen));
			++varNum;
			CLEARNSETSTR(varName, entity, "tc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, distribution_L(gen));
			++varNum;
			CLEARNSETSTR(varName, entity, "tc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, 
				std::round(static_cast<float>(distribution_L(gen)) / RAND_SMALL) * RAND_SMALL / RAND_LARGE);
			++varNum;
			CLEARNSETSTR(varName, entity, "tc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr,
				std::round(static_cast<float>(distribution_L(gen)) / RAND_SMALL) * RAND_SMALL / RAND_LARGE);
			++varNum;
			CLEARNSETSTR(varName, entity, "tc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, distribution_L(gen));
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<CircleCollider2DComponent>(entity))
		{
			CircleCollider2DComponent cc = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(entity);

			varNum = 0;
			CLEARNSETSTR(varName, entity, "ccc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, cc.getRadius());
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<BoxCollider2DComponent>(entity))
		{
			BoxCollider2DComponent bc = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);

			varNum = 0;
			CLEARNSETSTR(varName, entity, "bcc", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, static_cast<int>(bc.m_obb.getSize()));
			++varNum;
			for (std::vector<Vec2>::iterator it; (size_t)varNum < bc.m_obb.getSize(); ++it)
			{
				CLEARNSETSTR(varName, entity, "bcc", varNum);
				m_Serialiser.WriteToFile(fileName, cstr, it->x);
				++varNum;
				CLEARNSETSTR(varName, entity, "bcc", varNum);
				m_Serialiser.WriteToFile(fileName, cstr, it->y);
				++varNum;
			}
		}
	}

	RE_INFO("LEVEL SAVED");
}

std::vector<Entity> ObjectFactory::GetActiveEntity() const
{
	return m_activeEntities;
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