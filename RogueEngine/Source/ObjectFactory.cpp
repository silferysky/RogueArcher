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
	Vec2 WorldGravity{ 0.0f, 0.0f };

	//For Entity Count
	Entity entCount = level["EntityCount"].GetInt();

	//For Background
	Entity backgroundEnt = gEngine.m_coordinator.CreateEntity();
	std::string backgroundStr = level["BackgroundTexture"].GetString();

	SpriteComponent backgroundSprite = SpriteComponent();
	backgroundSprite.setTexture(backgroundStr.c_str());

	TransformComponent backgroundTransform = TransformComponent();
	backgroundTransform.setPosition(Vec2(0.0f, 0.0f));
	backgroundTransform.setScale(Vec2(18.0f, 12.0f));
	backgroundTransform.setRotation(0.0f);

	gEngine.m_coordinator.AddComponent(backgroundEnt, BoxCollider2DComponent());

	gEngine.m_coordinator.AddComponent(backgroundEnt, backgroundSprite);
	gEngine.m_coordinator.AddComponent(backgroundEnt, backgroundTransform);
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

		for (int index = 0; index < (int)LASTCOMP; ++index)
		{
			if (currentSignature.test(index))
			{
				switch (index)
				{
					case static_cast<int>(SPRITE) :
					{
						SpriteComponent spriteComponent{};
						CLEARNSETSTR(strstream, entity, "Sprite", 0);
						spriteComponent.Deserialize(level[cstr].GetString());

						gEngine.m_coordinator.AddComponent(curEnt, spriteComponent);
						break;
					}
					case static_cast<int>(RIGIDBODY) :
					{
						RigidbodyComponent rigidbodyComponent{};
						CLEARNSETSTR(strstream, entity, "Rigidbody", 0);
						rigidbodyComponent.Deserialize(level[cstr].GetString());

						gEngine.m_coordinator.AddComponent(curEnt, rigidbodyComponent);
						break;
					}
					case static_cast<int>(TRANSFORM) :
					{
						TransformComponent transformComponent{};
						CLEARNSETSTR(strstream, entity, "Transform", 0);
						transformComponent.Deserialize(level[cstr].GetString());

						gEngine.m_coordinator.AddComponent(curEnt, transformComponent);
						break;
					}
					case static_cast<int>(CIRCLECOLLIDER2D) :
					{
						CircleCollider2DComponent circleColliderComponent{};
						CLEARNSETSTR(strstream, entity, "CircleCollider", 0);
						circleColliderComponent.Deserialize(level[cstr].GetString());

						gEngine.m_coordinator.AddComponent(curEnt, circleColliderComponent);
						break;
					}
					case static_cast<int>(BOXCOLLIDER2D) :
					{
						BoxCollider2DComponent boxColliderComponent{};
						CLEARNSETSTR(strstream, entity, "BoxCollider", 0);
						boxColliderComponent.Deserialize(level[cstr].GetString());

						gEngine.m_coordinator.AddComponent(curEnt, boxColliderComponent);
						break;
					}
					case static_cast<int>(PLAYERCONTROLLER) :
					{
						PlayerControllerComponent playerControllerComponent{};
						gEngine.m_coordinator.AddComponent(curEnt, playerControllerComponent);
						break;
					}
					default:
					{
						RE_CORE_WARN("OUT OF BOUNDS OBJECT COMPONENT LOADING");
						break;
					}
				}
			}
		}

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

	bool skipping = true;

	for (Entity entity : m_activeEntities)
	{
		if (skipping)
		{
			skipping = false;
			continue;
		}
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
			CLEARNSETSTR(varName, entity, "Sprite", varNum);
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
			CLEARNSETSTR(varName, entity, "Rigidbody", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getAcceleration().x);
			++varNum;
			CLEARNSETSTR(varName, entity, "Rigidbody", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getAcceleration().y);
			++varNum;
			CLEARNSETSTR(varName, entity, "Rigidbody", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getVelocity().x);
			++varNum;
			CLEARNSETSTR(varName, entity, "Rigidbody", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getVelocity().y);
			++varNum;
			CLEARNSETSTR(varName, entity, "Rigidbody", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getInvMass());
			++varNum;
			CLEARNSETSTR(varName, entity, "Rigidbody", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, r.getVolume());
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<TransformComponent>(entity))
		{
			TransformComponent& t = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
			UNREFERENCED_PARAMETER(t);

			//std::random_device rd;
			//std::mt19937 gen(rd());		//For random seed
			//std::uniform_int_distribution<> distribution_L(-RAND_LARGE, RAND_LARGE);
			//std::uniform_int_distribution<> distribution_S(-RAND_SMALL, RAND_SMALL);

			//Position, Scale and Rotation
			//Scale is set to 2 decimal places by getting a random int, cast as float,
			//divided by RAND_SMALL, rounded off, then divided by RAND_LARGE and multiply
			//by RAND_SMALL to offset original RAND_SMALL
			//This will give a number with 2 decimal places
			varNum = 0;
			CLEARNSETSTR(varName, entity, "Transform", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, t.getPosition().x);
			++varNum;
			CLEARNSETSTR(varName, entity, "Transform", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, t.getPosition().y);
			++varNum;
			CLEARNSETSTR(varName, entity, "Transform", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, t.getScale().x);
			++varNum;
			CLEARNSETSTR(varName, entity, "Transform", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, t.getScale().y);
			++varNum;
			CLEARNSETSTR(varName, entity, "Transform", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, t.getRotation());
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<CircleCollider2DComponent>(entity))
		{
			CircleCollider2DComponent cc = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(entity);

			varNum = 0;
			CLEARNSETSTR(varName, entity, "CircleCollider", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, cc.getRadius());
		}

		if (gEngine.m_coordinator.CheckIfComponentExists<BoxCollider2DComponent>(entity))
		{
			BoxCollider2DComponent bc = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(entity);

			varNum = 0;
			CLEARNSETSTR(varName, entity, "BoxCollider", varNum);
			m_Serialiser.WriteToFile(fileName, cstr, static_cast<int>(bc.OBB().getSize()));
			++varNum;
			for (std::vector<Vec2>::iterator it = bc.OBB().modelVerts().begin(); (size_t)varNum < bc.OBB().getSize(); ++it)
			{
				CLEARNSETSTR(varName, entity, "BoxCollider", varNum);
				m_Serialiser.WriteToFile(fileName, cstr, it->x);
				++varNum;
				CLEARNSETSTR(varName, entity, "BoxCollider", varNum);
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