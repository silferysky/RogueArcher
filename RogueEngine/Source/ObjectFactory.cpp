#pragma once

#include <bitset>
#include <random>
#include <vector>

#include "Main.h"
#include "ObjectFactory.h"
#include "Types.h"
#include "logger.h"
#include "EntityManager.h"
#include "FileIO.h"

#define RAND_LARGE 500
#define RAND_SMALL 5

void ObjectFactory::LoadLevel(const char* fileName)
{
	rapidjson::Document level = RESerialiser::DeserialiseFromFile(fileName);

	Signature currentSignature;
	std::ostringstream debugStr;

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
		std::ostringstream ostrstream;
		std::string stdstr, readstr;
		const char* cstr;
		Entity curEnt = gEngine.m_coordinator.CreateEntity();

		//Setting signature
		ostrstream << "Signature" << entity;
		SETSSTOSTR(ostrstream);
		currentSignature = level[cstr].GetInt();
		CLEARSTR(ostrstream);

		//Setting deserializable string
		stdstr = "Entity";
		ostrstream << stdstr << static_cast<int>(entity);
		SETSSTOSTR(ostrstream);
		ostrstream.str(level[cstr].GetString());

		FactoryLoadComponent(curEnt, currentSignature, ostrstream.str());
		m_activeEntities.push_back(curEnt);

		debugStr << "Entity " << curEnt << "'s Signature: " << gEngine.m_coordinator.GetEntityManager().GetSignature(curEnt).to_ulong();
		RE_INFO(debugStr.str());
		CLEARSTR(debugStr);
	}
	RE_INFO("LEVEL LOADED");
	debugStr << entCount << " ENTITIES LOADED";
	RE_INFO(debugStr.str());

	//Clone(1);
	
}

void ObjectFactory::SaveLevel(const char* fileName)
{
	//Minus one off due to Background being part of the list as well.
	//Background is unique and will not be counted to the entCount
	Entity entCount = static_cast<Entity>(m_activeEntities.size() - 1);
	EntityManager* em = &gEngine.m_coordinator.GetEntityManager();
	int intVar = (int)entCount;
	RESerialiser::WriteToFile(fileName, "EntityCount", &intVar);

	bool writingBackground = true;

	for (Entity curEntity : m_activeEntities)
	{
		//Skips background layer
		if (writingBackground)
		{
			writingBackground = false;
			continue;
		}

		//Entity value acts as the value to store (-1 because of background)
		Entity entityVal = curEntity - 1;
		std::ostringstream strstream;
		std::string stdstr;
		const char* cstr;
		Signature currentSignature = em->GetSignature(curEntity);

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		strstream << "Signature" << entityVal;
		stdstr = strstream.str();
		cstr = stdstr.c_str();
		intVar = static_cast<int>(currentSignature.to_ulong());
		RESerialiser::WriteToFile(fileName, cstr, &intVar);
		CLEARSTR(strstream);
		
		for (int index = 0; index != LASTCOMP;)
		{
			if (currentSignature.test(index))
				switch (index)
				{
					case static_cast<int>(SPRITE) :
					{
						strstream << "Sprite{" << gEngine.m_coordinator.GetComponent<SpriteComponent>(curEntity).Serialize().c_str() << "}";
						break;
					}
					case static_cast<int>(RIGIDBODY) :
					{
						strstream << "Rigidbody{" << gEngine.m_coordinator.GetComponent<RigidbodyComponent>(curEntity).Serialize().c_str() << "}";
						break;
					}
					case static_cast<int>(TRANSFORM) :
					{
						strstream << "Transform{" << gEngine.m_coordinator.GetComponent<TransformComponent>(curEntity).Serialize() << "}";
						break;
					}
					case static_cast<int>(CIRCLECOLLIDER2D) :
					{
						strstream << "CircleCollider{" << gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(curEntity).Serialize() << "}";
						break;
					}
					case static_cast<int>(BOXCOLLIDER2D) :
					{
						strstream << "BoxCollider{" << gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(curEntity).Serialize() << "}";
						break;
					}
					case static_cast<int>(PLAYERCONTROLLER) :
					{
						strstream << "PlayerController{" << gEngine.m_coordinator.GetComponent<PlayerControllerComponent>(curEntity).Serialize() << "}";
						break;
					}
					case static_cast<int>(LOGIC) :
					{
						strstream << "LogicComponent{" << gEngine.m_coordinator.GetComponent<LogicComponent>(curEntity).Serialize() << "}";
						break;
					}
					default:
					{
						RE_CORE_WARN("OUT OF BOUNDS OBJECT COMPONENT SAVING");
						break;
					}
				}

			//Does checker and incrementing here
			if (++index != (COMPONENTID)LASTCOMP)
				strstream << "|";
		} //End of for loop strstream adding

		SETSSTOSTR(strstream);
		CLEARSTR(strstream);
		strstream << "Entity" << entityVal;
		RESerialiser::WriteToFile(fileName, strstream.str().c_str(), cstr);
	}

	RE_INFO("LEVEL SAVED");
}

void ObjectFactory::LoadArchetypes(const char* fileName)
{
}

void ObjectFactory::SaveArchetypes(const char* fileName)
{
}

void ObjectFactory::Clone(Entity toClone)
{
	Signature toCloneSignature = gEngine.m_coordinator.GetEntityManager().GetSignature(toClone);
	Entity clonedEntity = gEngine.m_coordinator.CreateEntity();
	for (int index = 0; index != LASTCOMP; ++index)
	{
		if (toCloneSignature.test(index))
		{
			switch (index)
			{
				case static_cast<int>(SPRITE) :
				{
					gEngine.m_coordinator.CopyComponent<SpriteComponent>(clonedEntity, toClone);
					break;
				}
				case static_cast<int>(RIGIDBODY) :
				{
					gEngine.m_coordinator.CopyComponent<RigidbodyComponent>(clonedEntity, toClone);
					break;
				}
				case static_cast<int>(TRANSFORM) :
				{
					gEngine.m_coordinator.CopyComponent<TransformComponent>(clonedEntity, toClone);
					break;
				}
				case static_cast<int>(CIRCLECOLLIDER2D) :
				{
					gEngine.m_coordinator.CopyComponent<CircleCollider2DComponent>(clonedEntity, toClone);
					break;
				}
				case static_cast<int>(BOXCOLLIDER2D) :
				{
					gEngine.m_coordinator.CopyComponent<BoxCollider2DComponent>(clonedEntity, toClone);
					break;
				}
				case static_cast<int>(PLAYERCONTROLLER) :
				{
					gEngine.m_coordinator.CopyComponent<PlayerControllerComponent>(clonedEntity, toClone);
					break;
				}
				case static_cast<int>(LOGIC) :
				{
					gEngine.m_coordinator.CopyComponent<LogicComponent>(clonedEntity, toClone);
					break;
				}
				default:
				{
					RE_CORE_WARN("OUT OF BOUNDS INDEX TO CLONE");
					break;
				}
			}
		}
	}
	m_activeEntities.push_back(clonedEntity);
	
}

void ObjectFactory::Clone(const char* archetype)
{
	//If the key exists
	if (m_archetypes.count(archetype))
	{
		std::istringstream istrstream;
		std::string readstr;

		std::getline(istrstream, readstr, '{');
		std::getline(istrstream, readstr, '}');

		Entity curEnt = gEngine.m_coordinator.CreateEntity();
		Signature curSignature = std::stoul(readstr);

		//Does the actual clone
		std::string toDeserialise = m_archetypes[archetype];
		FactoryLoadComponent(curEnt, curSignature, toDeserialise);
	}
}

std::vector<Entity> ObjectFactory::GetActiveEntity() const
{
	return m_activeEntities;
}

void ObjectFactory::FactoryLoadComponent(Entity curEnt, Signature signature, std::string value)
{
	std::istringstream istrstream(value);
	std::string readstr;
	for (int index = 0; index < (int)LASTCOMP; ++index)
	{
		if (signature.test(index))
		{
			//Does this twice to skip the name line
			std::getline(istrstream, readstr, '{');
			std::getline(istrstream, readstr, '}');

			switch (index)
			{
				case static_cast<int>(SPRITE) :
				{
					gEngine.m_coordinator.LoadComponent<SpriteComponent>(curEnt, readstr);
					break;
				}
				case static_cast<int>(RIGIDBODY) :
				{
					gEngine.m_coordinator.LoadComponent<RigidbodyComponent>(curEnt, readstr);
					break;
				}
				case static_cast<int>(TRANSFORM) :
				{
					gEngine.m_coordinator.LoadComponent<TransformComponent>(curEnt, readstr);
					break;
				}
				case static_cast<int>(CIRCLECOLLIDER2D) :
				{
					gEngine.m_coordinator.LoadComponent<CircleCollider2DComponent>(curEnt, readstr);
					break;
				}
				case static_cast<int>(BOXCOLLIDER2D) :
				{
					gEngine.m_coordinator.LoadComponent<BoxCollider2DComponent>(curEnt, readstr);
					break;
				}
				case static_cast<int>(PLAYERCONTROLLER) :
				{
					gEngine.m_coordinator.LoadComponent<PlayerControllerComponent>(curEnt, readstr);
					break;
				}
				case static_cast<int>(LOGIC) :
				{
					gEngine.m_coordinator.LoadComponent<LogicComponent>(curEnt, readstr);
					break;
				}
				default:
				{
					RE_CORE_WARN("OUT OF BOUNDS INDEX TO CLONE");
					break;
				}
			}
		}
	}
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