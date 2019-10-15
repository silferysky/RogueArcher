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
	std::stringstream debugStr;

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
		std::istringstream istrstream;
		std::string stdstr, readstr;
		const char* cstr;
		Entity curEnt = gEngine.m_coordinator.CreateEntity();

		//cstr will go out of scope if you choose to do strstream.str().c_str()
		//This is the proper (Non macro) way of setting the string
		ostrstream << "Signature" << entity;

		stdstr = ostrstream.str();
		cstr = stdstr.c_str();
		currentSignature = level[cstr].GetInt();

		ostrstream.clear();
		ostrstream.str("");

		stdstr = "Entity";
		ostrstream << stdstr << static_cast<int>(entity);
		SETSSTOSTR(ostrstream);

		istrstream.str(level[cstr].GetString());

		for (int index = 0; index < (int)LASTCOMP; ++index)
		{
			if (currentSignature.test(index))
			{
				//Does this twice to skip the name line
				std::getline(istrstream, readstr, '{');
				std::getline(istrstream, readstr, '}');
				
				CreateComponent(curEnt, index)->Deserialize(readstr);
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

	Clone(1);
	
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
					SpriteComponent* newComponent = dynamic_cast<SpriteComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<SpriteComponent>(toClone);
					break;
				}
				case static_cast<int>(RIGIDBODY) :
				{
					RigidbodyComponent* newComponent = dynamic_cast<RigidbodyComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<RigidbodyComponent>(toClone);
					break;
				}
				case static_cast<int>(TRANSFORM) :
				{
					TransformComponent* newComponent = dynamic_cast<TransformComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<TransformComponent>(toClone);
					break;
				}
				case static_cast<int>(CIRCLECOLLIDER2D) :
				{
					CircleCollider2DComponent* newComponent = dynamic_cast<CircleCollider2DComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(toClone);
					break;
				}
				case static_cast<int>(BOXCOLLIDER2D) :
				{
					BoxCollider2DComponent* newComponent = dynamic_cast<BoxCollider2DComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(toClone);
					break;
				}
				case static_cast<int>(PLAYERCONTROLLER) :
				{
					PlayerControllerComponent* newComponent = dynamic_cast<PlayerControllerComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<PlayerControllerComponent>(toClone);
					break;
				}
				case static_cast<int>(LOGIC) :
				{
					LogicComponent* newComponent = dynamic_cast<LogicComponent*>(CreateComponent(clonedEntity, index));
					*newComponent = gEngine.m_coordinator.GetComponent<LogicComponent>(toClone);
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

std::vector<Entity> ObjectFactory::GetActiveEntity() const
{
	return m_activeEntities;
}

BaseComponent* ObjectFactory::CreateComponent(Entity owner, ComponentType index)
{
	BaseComponent* componentToReturn = nullptr;
	switch (index)
	{
		case static_cast<int>(SPRITE) :
		{
			SpriteComponent spriteComponent{};
			gEngine.m_coordinator.AddComponent(owner, spriteComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<SpriteComponent>(owner);
			break;
		}
		case static_cast<int>(RIGIDBODY) :
		{
			RigidbodyComponent rigidbodyComponent{};
			gEngine.m_coordinator.AddComponent(owner, rigidbodyComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<RigidbodyComponent>(owner);
			break;
		}
		case static_cast<int>(TRANSFORM) :
		{
			TransformComponent transformComponent{};
			gEngine.m_coordinator.AddComponent(owner, transformComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<TransformComponent>(owner);
			break;
		}
		case static_cast<int>(CIRCLECOLLIDER2D) :
		{
			CircleCollider2DComponent circleColliderComponent{};
			gEngine.m_coordinator.AddComponent(owner, circleColliderComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<CircleCollider2DComponent>(owner);
			break;
		}
		case static_cast<int>(BOXCOLLIDER2D) :
		{
			BoxCollider2DComponent boxColliderComponent{};
			gEngine.m_coordinator.AddComponent(owner, boxColliderComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<BoxCollider2DComponent>(owner);
			break;
		}
		case static_cast<int>(PLAYERCONTROLLER) :
		{
			PlayerControllerComponent playerControllerComponent{};
			gEngine.m_coordinator.AddComponent(owner, playerControllerComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<PlayerControllerComponent>(owner);
			break;
		}
		case static_cast<int>(LOGIC) :
		{
			LogicComponent logicComponent{};
			gEngine.m_coordinator.AddComponent(owner, logicComponent);
			componentToReturn = &gEngine.m_coordinator.GetComponent<LogicComponent>(owner);
			break;
		}
		default:
		{
			RE_CORE_WARN("OUT OF BOUNDS INDEX TO CLONE");
			break;
		}
	}
	return componentToReturn;
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