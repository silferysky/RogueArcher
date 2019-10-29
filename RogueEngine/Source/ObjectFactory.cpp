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
#include "EditorHierarchyInfo.h"

namespace Rogue
{
#define RAND_LARGE 500
#define RAND_SMALL 5

	void ObjectFactory::LoadLevel(const char* fileName)
	{
		rapidjson::Document level = RESerialiser::DeserialiseFromFile(fileName);

		Signature currentSignature;
		std::ostringstream debugStr, ostrstream;
		std::string stdstr, readstr;
		const char* cstr;

		//For Entity Count
		m_maxEntityCount = level["MaxEntityCount"].GetInt();
		Entity entCount = level["EntityCount"].GetInt();

		//For Background
		Entity backgroundEnt = g_engine.m_coordinator.CreateEntity();
		std::string backgroundStr = level["BackgroundTexture"].GetString();
		SpriteComponent backgroundSprite = SpriteComponent();
		backgroundSprite.setTexture(backgroundStr.c_str());
		backgroundSprite.setTexturePath(backgroundStr.c_str());

		TransformComponent backgroundTransform = TransformComponent();
		backgroundTransform.setPosition(Vec2(0.0f, 0.0f));
		backgroundTransform.setScale(Vec2(GetWindowWidth(g_engine.GetWindowHandler()), GetWindowHeight(g_engine.GetWindowHandler())));
		backgroundTransform.setRotation(0.0f);

		g_engine.m_coordinator.AddComponent(backgroundEnt, backgroundSprite);
		g_engine.m_coordinator.AddComponent(backgroundEnt, backgroundTransform);
		CREATE_HIERARCHY_OBJ(backgroundEnt, ostrstream);

		for (Entity entity = 0; entity < entCount; ++entity)
		{
			Entity curEnt = g_engine.m_coordinator.CreateEntity();

			//Setting signature
			CLEARSTR(ostrstream);
			ostrstream << "Signature" << entity;
			SETSSTOSTR(ostrstream);
			currentSignature = level[cstr].GetInt();

			//Setting deserializable string
			CLEARSTR(ostrstream);
			stdstr = "Entity";
			ostrstream << stdstr << static_cast<int>(entity);
			SETSSTOSTR(ostrstream);
			ostrstream.str(level[cstr].GetString());

			FactoryLoadComponent(curEnt, currentSignature, ostrstream.str());

			CREATE_HIERARCHY_OBJ(curEnt, ostrstream);

			debugStr << "Entity " << curEnt << "'s Signature: " << g_engine.m_coordinator.GetEntityManager().GetSignature(curEnt).to_ulong();
			RE_INFO(debugStr.str());
			CLEARSTR(debugStr);
		}
		RE_INFO("LEVEL LOADED");
		debugStr << entCount << " ENTITIES LOADED";
		RE_INFO(debugStr.str());

	}

	void ObjectFactory::SaveLevel(const char* fileName)
	{
		std::ostringstream strstream;
		std::string stdstr;
		const char* cstr;

		//Minus one off due to Background being part of the list as well.
		//Background is unique and will not be counted to the entCount
		Entity entCount = 0;
		if (g_engine.m_coordinator.GetActiveObjects().size() != 0)
			entCount = static_cast<Entity>(g_engine.m_coordinator.GetActiveObjects().size() - 1);
		EntityManager* em = &g_engine.m_coordinator.GetEntityManager();
		int intVar = static_cast<int>(entCount);
		RESerialiser::WriteToFile(fileName, "EntityCount", &intVar);

		bool writingBackground = true;
		Entity entityVal = 0;

		for (HierarchyInfo curHierarchy : g_engine.m_coordinator.GetActiveObjects())
		{
			Entity curEntity = curHierarchy.m_Entity;
			//Skips background layer
			if (writingBackground)
			{
				if (g_engine.m_coordinator.CheckIfComponentExists<SpriteComponent>(curHierarchy.m_Entity))
				{
					std::string backgroundStr(g_engine.m_coordinator.GetComponent<SpriteComponent>(curHierarchy.m_Entity).Serialize().c_str());
					RESerialiser::WriteToFile(fileName, "BackgroundTexture", backgroundStr.c_str());
				}

				writingBackground = false;
				continue;
			}

			//Entity value acts as the value to store (-1 because of background)
			Signature currentSignature = em->GetSignature(curHierarchy.m_Entity);

			//cstr will go out of scope if you choose to do strstream.str().c_str()
			//This is the proper (Non macro) way of setting the string
			CLEARSTR(strstream);
			strstream << "Signature" << entityVal;
			SETSSTOSTR(strstream);
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
							strstream << "Sprite{" << g_engine.m_coordinator.GetComponent<SpriteComponent>(curEntity).Serialize().c_str() << "}";
							break;
						}
						case static_cast<int>(RIGIDBODY) :
						{
							strstream << "Rigidbody{" << g_engine.m_coordinator.GetComponent<RigidbodyComponent>(curEntity).Serialize().c_str() << "}";
							break;
						}
						case static_cast<int>(TRANSFORM) :
						{
							strstream << "Transform{" << g_engine.m_coordinator.GetComponent<TransformComponent>(curEntity).Serialize() << "}";
							break;
						}
						case static_cast<int>(CIRCLECOLLIDER2D) :
						{
							strstream << "CircleCollider{" << g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(curEntity).Serialize() << "}";
							break;
						}
						case static_cast<int>(BOXCOLLIDER2D) :
						{
							strstream << "BoxCollider{" << g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(curEntity).Serialize() << "}";
							break;
						}
						case static_cast<int>(PLAYERCONTROLLER) :
						{
							strstream << "PlayerController{" << g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(curEntity).Serialize() << "}";
							break;
						}
						case static_cast<int>(LOGIC) :
						{
							strstream << "LogicComponent{" << g_engine.m_coordinator.GetComponent<LogicComponent>(curEntity).Serialize() << "}";
							break;
						}
						case static_cast<int>(STATS) :
						{
							strstream << "StatsComponent{" << g_engine.m_coordinator.GetComponent<StatsComponent>(curEntity).Serialize() << "}";
							break;
						}
						case static_cast<int>(ANIMATION) :
						{
							strstream << "Animation{" << g_engine.m_coordinator.GetComponent<AnimationComponent>(curEntity).Serialize().c_str() << "}";
							break;
						}
						case static_cast<int>(CAMERA) :
						{
							strstream << "Camera{" << g_engine.m_coordinator.GetComponent<CameraComponent>(curEntity).Serialize().c_str() << "}";
							break;
						}
						case static_cast<int>(AUDIOEMITTER) :
						{
							strstream << "Audio{" << g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(curEntity).Serialize().c_str() << "}";
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
			++entityVal;
		}

		RE_INFO("LEVEL SAVED");
	}

	void ObjectFactory::LoadArchetypes(const char* fileName)
	{
		std::stringstream strstream;
		std::string stdstr, readstr;	//stdstr for name, readstr for value
		const char* cstr;
		Signature curSignature;

		rapidjson::Document level = RESerialiser::DeserialiseFromFile(fileName);
		m_maxArchetypeCount = level["EntityCount"].GetInt();
		Entity entCount = level["EntityCount"].GetInt();

		for (Entity count = 0; count < entCount; ++count)
		{
			//For Signature
			CLEARSTR(strstream);
			readstr = "Signature";
			strstream << readstr << static_cast<int>(count);
			SETSSTOSTR(strstream);
			curSignature = level[cstr].GetInt();

			//For value
			CLEARSTR(strstream);
			readstr = "Entity";
			strstream << readstr << static_cast<int>(count);
			SETSSTOSTR(strstream);
			strstream.str(level[cstr].GetString());
			std::getline(strstream, stdstr, '{');
			std::getline(strstream, stdstr, '}');
			std::getline(strstream, readstr);
			//Leftover '|' character will be ignored based on the getlines
			SetArchetype(stdstr, readstr, curSignature);
		}
		RE_CORE_INFO("Archetypes loaded");
		//RE_CORE_INFO(m_archetypes.size());
		//RE_CORE_INFO(m_archetypeSignature.size());
	}

	void ObjectFactory::SaveArchetypes(const char* fileName)
	{
		std::ostringstream strstream;
		std::string stdstr;
		const char* cstr;

		//For EntCount
		Entity entCount = static_cast<Entity>(m_archetypes.size());
		int intVar = (int)entCount;
		RESerialiser::WriteToFile(fileName, "EntityCount", &intVar);

		int position = 0;
		for (auto it = m_archetypes.begin(); it != m_archetypes.end(); ++it, ++position)
		{
			//Writing signature value
			CLEARSTR(strstream);
			strstream << "Signature" << position;
			SETSSTOSTR(strstream);
			intVar = static_cast<int>(m_archetypeSignature[it->first].to_ulong());
			RESerialiser::WriteToFile(fileName, cstr, &intVar);

			//Writing Actual Value
			CLEARSTR(strstream);
			strstream << "Name{" << it->first << "}" << it->second;
			SETSSTOSTR(strstream); //Sets cstr
			CLEARSTR(strstream);
			strstream << "Entity" << position;
			RESerialiser::WriteToFile(fileName, strstream.str().c_str(), cstr);
		}
	}

	void ObjectFactory::LoadLevelFiles(const char* fileName)
	{
		std::stringstream strstream;
		std::string stdstr;
		rapidjson::Document level = RESerialiser::DeserialiseFromFile(fileName);
		strstream << level["Files"].GetString();

		while(std::getline(strstream, stdstr, ';'))
			g_engine.m_coordinator.GetSceneManager().AddToLoadedLevels(stdstr);

		m_maxFileCount = g_engine.m_coordinator.GetSceneManager().GetLoadedLevels().size();
	}

	void ObjectFactory::SaveLevelFiles(const char* fileName)
	{
		std::stringstream strstream;
		
		for (auto& fileObjects : g_engine.m_coordinator.GetSceneManager().GetLoadedLevels())
		{
			strstream << fileObjects << ";";
		}

		RESerialiser::WriteToFile(fileName, "Files", strstream.str().c_str());
	}

	void ObjectFactory::Clone(Entity toClone)
	{
		Signature toCloneSignature = g_engine.m_coordinator.GetEntityManager().GetSignature(toClone);
		Entity clonedEntity = g_engine.m_coordinator.CreateEntity();
		for (int index = 0; index != LASTCOMP; ++index)
		{
			if (toCloneSignature.test(index))
			{
				switch (index)
				{
					case static_cast<int>(SPRITE) :
					{
						g_engine.m_coordinator.CopyComponent<SpriteComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(RIGIDBODY) :
					{
						g_engine.m_coordinator.CopyComponent<RigidbodyComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(TRANSFORM) :
					{
						g_engine.m_coordinator.CopyComponent<TransformComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(CIRCLECOLLIDER2D) :
					{
						g_engine.m_coordinator.CopyComponent<CircleCollider2DComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(BOXCOLLIDER2D) :
					{
						g_engine.m_coordinator.CopyComponent<BoxCollider2DComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(PLAYERCONTROLLER) :
					{
						g_engine.m_coordinator.CopyComponent<PlayerControllerComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(LOGIC) :
					{
						g_engine.m_coordinator.CopyComponent<LogicComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(ANIMATION) :
					{
						g_engine.m_coordinator.CopyComponent<AnimationComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(CAMERA) :
					{
						g_engine.m_coordinator.CopyComponent<CameraComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(AUDIOEMITTER) :
					{
						g_engine.m_coordinator.CopyComponent<AudioEmitterComponent>(clonedEntity, toClone);
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
		std::ostringstream ostrstream;
		std::string stdstr;
		CREATE_HIERARCHY_OBJ(clonedEntity, ostrstream);

	}

	void ObjectFactory::Clone(const char* archetype)
	{
		std::ostringstream ostrstream;
		std::string stdstr;

		//If the key exists
		if (m_archetypes.count(archetype))
		{
			Entity curEnt = g_engine.m_coordinator.CreateEntity();
			Signature curSignature = m_archetypeSignature[archetype];

			//Does the actual clone
			std::string toDeserialise = m_archetypes[archetype];
			FactoryLoadComponent(curEnt, curSignature, toDeserialise);
			CREATE_HIERARCHY_OBJ(curEnt, ostrstream);
		}
	}

	bool ObjectFactory::CheckFileTooSmall(size_t type, size_t size)
	{
		if (type == FILETYPE_LEVEL)
			//If the max entity count is 0 (File is empty), always return true
			if (m_maxEntityCount == 0)
				return true;
			else
				return size > m_maxEntityCount + 1; // +1 for background
		else if (type == FILETYPE_ARCHETYPE)
			return size > m_maxArchetypeCount;
		else
			return size > m_maxFileCount;
	}

	void ObjectFactory::ResetMaxEntity()
	{
		m_maxEntityCount = 0;
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
						g_engine.m_coordinator.LoadComponent<SpriteComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(RIGIDBODY) :
					{
						g_engine.m_coordinator.LoadComponent<RigidbodyComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(TRANSFORM) :
					{
						g_engine.m_coordinator.LoadComponent<TransformComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(CIRCLECOLLIDER2D) :
					{
						g_engine.m_coordinator.LoadComponent<CircleCollider2DComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(BOXCOLLIDER2D) :
					{
						g_engine.m_coordinator.LoadComponent<BoxCollider2DComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(PLAYERCONTROLLER) :
					{
						g_engine.m_coordinator.LoadComponent<PlayerControllerComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(LOGIC) :
					{
						g_engine.m_coordinator.LoadComponent<LogicComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(STATS) :
					{
						g_engine.m_coordinator.LoadComponent<StatsComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(ANIMATION) :
					{
						g_engine.m_coordinator.LoadComponent<AnimationComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(CAMERA) :
					{
						g_engine.m_coordinator.LoadComponent<CameraComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(AUDIOEMITTER) :
					{
						g_engine.m_coordinator.LoadComponent<AudioEmitterComponent>(curEnt, readstr);
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

	void ObjectFactory::SetArchetype(std::string archetypeName, std::string archetypeValue, Signature archetypeSignature)
	{
		m_archetypes.insert({ archetypeName, archetypeValue });
		m_archetypeSignature.insert({ archetypeName, archetypeSignature });
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
}