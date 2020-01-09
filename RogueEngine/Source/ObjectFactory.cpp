/* Start Header ************************************************************************/
/*!
\file           ObjectFactory.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ObjectFactory

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "Precompiled.h"

#include "Main.h"
#include "ObjectFactory.h"
#include "Types.h"
#include "logger.h"
#include "EntityManager.h"
#include "FileIO.h"
#include "EditorHierarchyInfo.h"
#include "CameraManager.h"

namespace Rogue
{
#define RAND_LARGE 500
#define RAND_SMALL 5

	void ObjectFactory::LoadLevel(const char* fileName)
	{
		rapidjson::Document level = RESerialiser::DeserialiseFromFile(fileName);

		Signature currentSignature;
		std::ostringstream debugStr, ostrstream;
		std::istringstream istrstream;
		std::string stdstr, tagstr, readstr;
		const char* cstr;

		//For Camera
		Vec2 cameraMin(level["CameraMinX"].GetFloat(), level["CameraMinY"].GetFloat());
		Vec2 cameraMax(level["CameraMaxX"].GetFloat(), level["CameraMaxY"].GetFloat());
		float cameraZoom(level["CameraZoom"].GetFloat());
		CameraManager::instance().SetCameraMin(cameraMin);
		CameraManager::instance().SetCameraMax(cameraMax);
		CameraManager::instance().SetLevelCameraZoom(cameraZoom);
		CameraManager::instance().SetCameraZoom(cameraZoom);

		//For Entity Count
		m_maxEntityCount = level["MaxEntityCount"].GetInt();
		Entity entCount = level["EntityCount"].GetInt();
		
		//For Parent/Child iterator
		std::queue<Entity> loadedQueue;

		for (Entity entity = 0; entity < entCount; ++entity)
		{
			Entity curEnt = g_engine.m_coordinator.CreateEntity();
			loadedQueue.push(curEnt);

			//Setting signature
			CLEARSTR(ostrstream);
			ostrstream << "Signature" << entity;
			SETSSTOSTR(ostrstream);
			currentSignature = level[cstr].GetInt();

			//Setting deserializable string
			CLEARSTR(ostrstream);
			ostrstream << "Entity" << static_cast<int>(entity);
			SETSSTOSTR(ostrstream);
			istrstream.str(level[cstr].GetString());

			//For entity name
			//Does this twice to skip the name line, twice for tag
			istrstream.clear();
			stdstr = "";
			std::getline(istrstream, readstr, '{');
			std::getline(istrstream, readstr, '}');
			std::getline(istrstream, tagstr, '{');
			std::getline(istrstream, tagstr, '}');
			std::getline(istrstream, stdstr);

			FactoryLoadComponent(curEnt, currentSignature, stdstr);

			//Setting parent
			CLEARSTR(ostrstream);
			ostrstream << "EntityParent" << static_cast<int>(entity);
			Entity entityParent = level[ostrstream.str().c_str()].GetInt();

			CREATE_HIERARCHY_OBJ(curEnt, readstr, tagstr, loadedQueue.front() + entityParent);
			//HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(curEnt);
			//debugStr << "Entity " << info.m_Entity << ":" << info.m_objectName << " has parent " << info.m_parent;
			//RE_INFO(debugStr.str());
			//CLEARSTR(debugStr);

			debugStr << "Entity " << curEnt << "'s Signature: " << g_engine.m_coordinator.GetEntityManager().GetSignature(curEnt).to_ulong();
			RE_INFO(debugStr.str());
			CLEARSTR(debugStr);
		}


		//Assigning child
		if (loadedQueue.size())
		{
			Entity firstLoadedEnt = loadedQueue.front();
			while (loadedQueue.size())
			{
				Entity loadedEnt = loadedQueue.front();
				HierarchyInfo& childInfo = g_engine.m_coordinator.GetHierarchyInfo(loadedEnt);
				//debugStr << "Entity " << childInfo.m_Entity << ":" << childInfo.m_objectName << " has parent " << childInfo.m_parent;
				//RE_INFO(debugStr.str());
				//CLEARSTR(debugStr);

				//Check if parent of itself or  out of bounds (AKA parent is default value)
				if (childInfo.m_parent == loadedEnt || childInfo.m_parent > loadedEnt + loadedQueue.size() || childInfo.m_parent < firstLoadedEnt)
				{
					childInfo.m_parent = -1;
					loadedQueue.pop();
					continue;
				}

				HierarchyInfo& parentInfo = g_engine.m_coordinator.GetHierarchyInfo(childInfo.m_parent);
				parentInfo.m_children.push_back(loadedEnt);
				loadedQueue.pop();
				//debugStr << "Entity " << parentInfo.m_Entity << ":" << parentInfo.m_objectName << " has child " << loadedEnt;
				//RE_INFO(debugStr.str());
				//CLEARSTR(debugStr);
			}
		}
		//for (; entityIt != entityEnd; ++entityIt)
		//{
		//	HierarchyInfo& childInfo = g_engine.m_coordinator.GetHierarchyInfo(entityIt);

		//	//Check if out of bounds (AKA parent is default value)
		//	if (childInfo.m_parent > entityEnd || childInfo.m_parent == entityIt)
		//	{
		//		childInfo.m_parent = -1;
		//		continue;
		//	}

		//	HierarchyInfo& parentInfo = g_engine.m_coordinator.GetHierarchyInfo(childInfo.m_parent);
		//	parentInfo.m_children.push_back(entityIt);
		//	debugStr << "Entity " << parentInfo.m_Entity << ":" << parentInfo.m_objectName << " has child " << entityIt;
		//	RE_INFO(debugStr.str());
		//	//RE_INFO("HI");
		//}

		RE_INFO("LEVEL LOADED");
		debugStr << entCount << " ENTITIES LOADED";
		RE_INFO(debugStr.str());

	}

	void ObjectFactory::SaveLevel(const char* fileName)
	{
		std::ostringstream strstream;
		std::string stdstr;
		const char* cstr;

		//Camera Serialization
		Vec2 cameraMin = CameraManager::instance().GetCameraMin();
		Vec2 cameraMax = CameraManager::instance().GetCameraMax();
		float cameraZoom = CameraManager::instance().GetLevelCameraZoom();
		RESerialiser::WriteToFile(fileName, "CameraMinX", &cameraMin.x);
		RESerialiser::WriteToFile(fileName, "CameraMinY", &cameraMin.y);
		RESerialiser::WriteToFile(fileName, "CameraMaxX", &cameraMax.x);
		RESerialiser::WriteToFile(fileName, "CameraMaxY", &cameraMax.y);
		RESerialiser::WriteToFile(fileName, "CameraZoom", &cameraZoom);

		Entity entCount = 0;
		size_t skipCount = 0; //No need to skip MenuControllerSystem's UI since they are not created as activeobjects
		entCount = static_cast<Entity>(g_engine.m_coordinator.GetActiveObjects().size() - skipCount);

		/*size_t skipCount = g_engine.m_coordinator.GetSystem<MenuControllerSystem>()->GetUIMenuObjsSize();
		if (g_engine.m_coordinator.GetActiveObjects().size() - skipCount > 0)
			entCount = static_cast<Entity>(g_engine.m_coordinator.GetActiveObjects().size() - skipCount);*/

		EntityManager* em = &g_engine.m_coordinator.GetEntityManager();
		int intVar = static_cast<int>(entCount);
		RESerialiser::WriteToFile(fileName, "EntityCount", &intVar);

		bool writingBackground = true;
		entCount = 0; //Reset entCount for saving loop

		for (Entity& curEntity : g_engine.m_coordinator.GetActiveObjects())
		{
			if (skipCount)
			{
				--skipCount;
				continue;
			}
			HierarchyInfo& curHierarchy = g_engine.m_coordinator.GetHierarchyInfo(curEntity);
			//Background layer is unique
			/*if (writingBackground)
			{
				if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(curEntity))
				{
					std::string backgroundStr(g_engine.m_coordinator.GetComponent<SpriteComponent>(curEntity).Serialize());
					RESerialiser::WriteToFile(fileName, "BackgroundTexture", backgroundStr.c_str());
				}

				writingBackground = false;
				continue;
			}*/

			//Entity value acts as the value to store (-1 because of background)
			Signature currentSignature = em->GetSignature(curEntity);

			//cstr will go out of scope if you choose to do strstream.str().c_str()
			//This is the proper (Non macro) way of setting the string
			CLEARSTR(strstream);
			strstream << "Signature" << entCount;
			SETSSTOSTR(strstream);
			intVar = static_cast<int>(currentSignature.to_ulong());
			RESerialiser::WriteToFile(fileName, cstr, &intVar);

			CLEARSTR(strstream);
			strstream << SerializeComponents(curHierarchy);
			SETSSTOSTR(strstream);
			CLEARSTR(strstream); //Done this way so strstream's string is different from stdstr's string
			strstream << "Entity" << entCount;
			RESerialiser::WriteToFile(fileName, strstream.str().c_str(), cstr);

			CLEARSTR(strstream);
			strstream << "EntityParent" << entCount;
			SETSSTOSTR(strstream);
			int parentValue = curHierarchy.m_parent;
			//if (parentValue == MAX_ENTITIES)
			//	parentValue = -1;
			RESerialiser::WriteToFile(fileName, strstream.str().c_str(), &parentValue);

			++entCount;
		}

		RE_INFO("LEVEL SAVED");
	}

	void ObjectFactory::LoadArchetypes(const char* fileName)
	{
		rapidjson::Document archetypeList = RESerialiser::DeserialiseFromFile(fileName);
		std::istringstream istrstream(archetypeList["ArchetypeList"].GetString());
		std::ostringstream ostrstream;
		std::string stdstr;
		Signature curSignature;

		while (std::getline(istrstream, stdstr, ';'))
		{
			CLEARSTR(ostrstream);
			ostrstream << "Resources/Archetypes/" << stdstr << ".json";

			rapidjson::Document archetypeFile = RESerialiser::DeserialiseFromFile(ostrstream.str().c_str());
			curSignature = archetypeFile["Signature"].GetInt();
			AddToArchetypes(stdstr, curSignature, archetypeFile["Entity"].GetString());
		}


		/*m_maxArchetypeCount = level["EntityCount"].GetInt();
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
		RE_CORE_INFO("Archetypes loaded");*/
	}

	void ObjectFactory::SaveArchetypeList(const char* fileName)
	{
		std::ostringstream strstream;

		for (auto& set : m_archetypes)
		{
			strstream << set.first << ";";
		}

		RESerialiser::WriteToFile(fileName, "ArchetypeList", strstream.str().c_str());

		/*/For EntCount
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
		}*/
	}

	void ObjectFactory::SaveArchetype(std::string_view file)
	{
		auto iterator = m_archetypes.find(file.data());
		if (iterator == m_archetypes.end())
			return;
		
		std::ostringstream ostrstream;
		ostrstream << "Resources/Archetypes/" << file << ".json";

		HierarchyInfo info; //= g_engine.m_coordinator.GetHierarchyInfo();
		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
		int signatureInInt = -1;
		for (Entity& obj : activeObjects)
		{
			if (g_engine.m_coordinator.GetHierarchyInfo(obj).m_objectName == file)
			{
				info = g_engine.m_coordinator.GetHierarchyInfo(obj);
				signatureInInt = static_cast<int>(g_engine.m_coordinator.GetEntityManager().GetSignature(obj).to_ulong());
			}
		}
		
		//Safety check if entity not found
		if (signatureInInt == -1)
			return;

		RESerialiser::WriteToFile(ostrstream.str().c_str(), "Signature", &signatureInInt);
		RESerialiser::WriteToFile(ostrstream.str().c_str(), "Entity", SerializeComponents(info).c_str());
	}

	void ObjectFactory::AddToArchetypes(std::string_view archetypeName, Signature signature, std::string_view toDeserialize)
	{
		m_archetypes.insert({ archetypeName.data(), std::pair<Signature, std::string>(signature, toDeserialize.data()) });
	}

	void ObjectFactory::LoadLevelFiles(const char* fileName)
	{
		rapidjson::Document level = RESerialiser::DeserialiseFromFile(fileName);
		std::stringstream strstream;
		//std::istringstream istrstream(level["Sounds"].GetString());
		std::string stdstr;
		strstream << level["Files"].GetString();

		while(std::getline(strstream, stdstr, ';'))
			SceneManager::instance().AddToLoadedLevels(stdstr);

		SceneManager::instance().setCurrentFileName(SceneManager::instance().GetLoadedLevels().front());

		m_maxFileCount = SceneManager::instance().GetLoadedLevels().size();

		//CameraManager::instance().SetCameraMin(Vec2(level["CameraMinX"].GetFloat(), level["CameraMinY"].GetFloat()));
		//CameraManager::instance().SetCameraMax(Vec2(level["CameraMaxX"].GetFloat(), level["CameraMaxY"].GetFloat()));

		stdstr = std::string();

		auto& audioManager = AudioManager::instance();

		/*while (std::getline(istrstream, stdstr, ';'))
		{
			strstream.clear();
			strstream.str("");
			strstream << "Resources/Sounds/" << stdstr;

			audioManager.loadSound(strstream.str());
		}*/
		
	}

	void ObjectFactory::SaveLevelFiles(const char* fileName)
	{
		std::stringstream strstream;
		
		for (auto& fileObjects : SceneManager::instance().GetLoadedLevels())
		{
			strstream << fileObjects << ";";
		}

		RESerialiser::WriteToFile(fileName, "Files", strstream.str().c_str());
	}

	void ObjectFactory::Clone(Entity toClone)
	{
		/*Signature toCloneSignature = g_engine.m_coordinator.GetEntityManager().GetSignature(toClone);
		Entity clonedEntity = g_engine.m_coordinator.CreateEntity();

		//auto iterator = g_engine.m_coordinator.GetActiveObjects().
		//if (iterator == m_archetypes.end())
			//return;

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
					case static_cast<int>(COLLIDER) :
					{
						g_engine.m_coordinator.CopyComponent<ColliderComponent>(clonedEntity, toClone);
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
					case static_cast<int>(UI) :
					{
						g_engine.m_coordinator.CopyComponent<UIComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(TEXT) :
					{
						g_engine.m_coordinator.CopyComponent<TextComponent>(clonedEntity, toClone);
						break;
					}
					case static_cast<int>(BACKGROUND) :
					{
						g_engine.m_coordinator.CopyComponent<BackgroundComponent>(clonedEntity, toClone);
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
		ostrstream << "Game Object " << SceneManager::instance().GetObjectIterator();
		CREATE_HIERARCHY_OBJ(clonedEntity, ostrstream.str(), "");*/

	}

	Entity ObjectFactory::Clone(const char* archetype, bool createHierarchy)
	{
		//If the key exists
		if (m_archetypes.count(archetype))
		{
			std::ostringstream ostrstream;
			Entity curEnt = g_engine.m_coordinator.CreateEntity();
			Signature curSignature = m_archetypes[archetype].first;

			//Does the actual clone
			std::istringstream istrstream(m_archetypes[archetype].second);
			std::string toDeserialise, tagDeserialized;
			//To skip 2 things - name
			std::getline(istrstream, toDeserialise, '{');
			std::getline(istrstream, toDeserialise, '}');
			ostrstream << toDeserialise;// << SceneManager::instance().GetObjectIterator();
			std::getline(istrstream, tagDeserialized, '{');
			std::getline(istrstream, tagDeserialized, '}');
			//And get the rest of the details
			std::getline(istrstream, toDeserialise);
			FactoryLoadComponent(curEnt, curSignature, toDeserialise);

			if (createHierarchy)
			{
				CREATE_HIERARCHY_OBJ(curEnt, ostrstream.str(), tagDeserialized, -1);
			}

			return curEnt;
		}
		else
			return NULL;
	}

	bool ObjectFactory::CheckFileTooSmall(size_t type, size_t size)
	{
		if (type == FILETYPE_LEVEL)
			//If the max entity count is 0 (File is empty), always return true
			if (m_maxEntityCount == 0)
				return true;
			else
				return size > m_maxEntityCount;
		else if (type == FILETYPE_ARCHETYPE)
			return size > m_maxArchetypeCount;
		else
			return size > m_maxFileCount;
	}

	void ObjectFactory::ResetMaxEntity()
	{
		m_maxEntityCount = 0;
	}

	void ObjectFactory::AssignParentChild(HierarchyInfo& parent, HierarchyInfo& child)
	{
		parent.m_children.push_back(child.m_Entity);
		child.m_parent = parent.m_Entity;
	}

	std::string ObjectFactory::SerializeComponents(HierarchyInfo& hierarchyToSerialize)
	{
		std::ostringstream strstream;
		//For object name
		strstream << "Name{" << hierarchyToSerialize.m_objectName << "}|";
		strstream << "Tag{" << hierarchyToSerialize.m_tag << "}|";

		Signature currentSignature = g_engine.m_coordinator.GetEntityManager().GetSignature(hierarchyToSerialize.m_Entity);
		Entity& entityToSerialize = hierarchyToSerialize.m_Entity;
		for (int index = 0; index != LASTCOMP; ++index)
		{
			if (currentSignature.test(index))
				switch (index)
				{
					case static_cast<int>(SPRITE) :
					{
						strstream << "Sprite{" << g_engine.m_coordinator.GetComponent<SpriteComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(RIGIDBODY) :
					{
						strstream << "Rigidbody{" << g_engine.m_coordinator.GetComponent<RigidbodyComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(TRANSFORM) :
					{
						strstream << "Transform{" << g_engine.m_coordinator.GetComponent<TransformComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(CIRCLECOLLIDER2D) :
					{
						strstream << "BoundingCircle{" << g_engine.m_coordinator.GetComponent<CircleCollider2DComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(BOXCOLLIDER2D) :
					{
						strstream << "BoxCollider{" << g_engine.m_coordinator.GetComponent<BoxCollider2DComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(COLLIDER) :
					{
						strstream << "Collider{" << g_engine.m_coordinator.GetComponent<ColliderComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(PLAYERCONTROLLER) :
					{
						strstream << "PlayerController{" << g_engine.m_coordinator.GetComponent<PlayerControllerComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(LOGIC) :
					{
						strstream << "LogicComponent{" << g_engine.m_coordinator.GetComponent<LogicComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(STATS) :
					{
						strstream << "StatsComponent{" << g_engine.m_coordinator.GetComponent<StatsComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(ANIMATION) :
					{
						strstream << "Animation{" << g_engine.m_coordinator.GetComponent<AnimationComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(CAMERA) :
					{
						strstream << "Camera{" << g_engine.m_coordinator.GetComponent<CameraComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(AUDIOEMITTER) :
					{
						strstream << "Audio{" << g_engine.m_coordinator.GetComponent<AudioEmitterComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(UI) :
					{
						strstream << "UI{" << g_engine.m_coordinator.GetComponent<UIComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(CURSOR) :
					{
						strstream << "Cursor{" << g_engine.m_coordinator.GetComponent<CursorComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(TEXT) :
					{
						strstream << "Text{" << g_engine.m_coordinator.GetComponent<TextComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(BACKGROUND) :
					{
						strstream << "Background{" << g_engine.m_coordinator.GetComponent<BackgroundComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(PARTICLEEMITTER) :
					{
						strstream << "Particle Emitter{" << g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					case static_cast<int>(PARTICLE) :
					{
						strstream << "Particle{}";
						//strstream << "Particle{" << g_engine.m_coordinator.GetComponent<ParticleComponent>(entityToSerialize).Serialize() << "}";
						break;
					}
					default:
					{
						RE_CORE_WARN("OUT OF BOUNDS OBJECT COMPONENT SAVING");
						break;
					}
				}

			strstream << "|";
		} //End of for loop strstream adding
		//strstream.str().substr(0, strstream.str().size() - 1);
		return strstream.str();
	}

	std::string ObjectFactory::SerializeChildren(HierarchyInfo& entityHierarchy)
	{
		std::ostringstream strstream;
		
		for (Entity& child : entityHierarchy.m_children)
		{
			strstream << child << ";";
		}

		return strstream.str();
	}

	std::map<std::string, std::pair<Signature, std::string>> ObjectFactory::GetArchetypeMap() const
	{
		return m_archetypes;
	}

	void ObjectFactory::FactoryLoadComponent(Entity curEnt, Signature signature, std::string_view value)
	{
		std::istringstream istrstream(value.data());
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
					case static_cast<int>(COLLIDER) :
					{
						g_engine.m_coordinator.LoadComponent<ColliderComponent>(curEnt, readstr);
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
					case static_cast<int>(UI) :
					{
						g_engine.m_coordinator.LoadComponent<UIComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(CURSOR) :
					{
						g_engine.m_coordinator.LoadComponent<CursorComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(TEXT) :
					{
						g_engine.m_coordinator.LoadComponent<TextComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(BACKGROUND) :
					{
						g_engine.m_coordinator.LoadComponent<BackgroundComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(PARTICLEEMITTER) :
					{
						g_engine.m_coordinator.LoadComponent<ParticleEmitterComponent>(curEnt, readstr);
						break;
					}
					case static_cast<int>(PARTICLE) :
					{
						g_engine.m_coordinator.LoadComponent<ParticleComponent>(curEnt, readstr);
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
}