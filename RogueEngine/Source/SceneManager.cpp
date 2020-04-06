/* Start Header ************************************************************************/
/*!
\file           SceneManager.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for SceneManager

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "SceneManager.h"
#include "ObjectFactory.h"
#include "EditorHierarchyInfo.h"
#include "BasicIO.h"
#include "MenuControllerSystem.h"
#include "CameraManager.h"
#include "EditorCreateObjectCommand.h"
#include "PlayerStatusManager.h"
#include "GameEvent.h"
#include "PlayerStatusManager.h"
#include "EditorTileSet.h"

namespace Rogue
{
	SceneManager::SceneManager()
	:	m_objectFactory {std::make_unique<ObjectFactory>()},
		m_loadedLevels {std::vector<std::string>()},
		m_currentFileName { "Level 1.json"},
		m_sceneIterator{ 0 }
	{
		//LoadLevelFiles("Levels.json");
		//m_sceneIterator = m_loadedLevels.size();
	}

	SceneManager::~SceneManager()
	{
		//ClearActiveEntities();
	}

	std::string SceneManager::getCurrentFileName() const
	{
		return m_currentFileName;
	}

	void SceneManager::setCurrentFileName(std::string_view curFileName)
	{
		m_currentFileName = curFileName;
	}

	void SceneManager::ClearActiveEntities()
	{
		g_engine.m_coordinator.GetActiveObjects().clear();
	}

	void SceneManager::ClearAllEntities()
	{
		g_engine.m_coordinator.GetSystem<AudioSystem>()->ShutdownSounds();
		g_engine.m_coordinator.GetSystem<LogicSystem>()->ClearLogicInterface();
		AudioManager::instance().ClearAudioMap();
		ImGuiTileSet::instance().ClearTileset();
		g_engine.m_coordinator.DestroyAllEntity();
		//ClearActiveEntities();
	}

	void SceneManager::LoadLevelFiles(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/" << fileName;
		m_objectFactory->LoadLevelFiles(ostrstream.str().c_str());
		LoadLevel(m_currentFileName);
		m_sceneIterator = static_cast<unsigned int>(m_loadedLevels.size());
	}

	void SceneManager::SaveLevelFiles(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/" << fileName;
		m_objectFactory->SaveLevelFiles(ostrstream.str().c_str());
	}

	size_t SceneManager::GetEntToSaveInLevel() const
	{
		size_t entCount = 0;

		for (auto& ent : g_engine.m_coordinator.GetActiveObjects())
		{
			if (g_engine.m_coordinator.ComponentExists<SaveComponent>(ent))
				++entCount;
		}

		return entCount;
	}

	void SceneManager::LoadLevel(const std::string& fileName)
	{
#if ENABLE_CORE_LOGGING
		auto start = Timer::instance().GetCurrTime();
#endif

		if (PLAYER_STATUS.GetPlayerEntity() != MAX_ENTITIES)
		{
			//Telling PlayerControllerSystem to do full reset
			ResetGameEvent ev;
			ev.SetSystemReceivers((int)SystemID::id_PLAYERCONTROLLERSYSTEM);
			EventDispatcher::instance().AddEvent(ev);
		}

		//Setting up
		if (getCurrentFileName() != "Level 16.json")
			PLAYER_STATUS.SetLastLevel(getCurrentFileName());
		setCurrentFileName(fileName);
		ClearAllEntities();

		g_engine.m_coordinator.ResetEntityQueue();

		CameraManager::instance().SetCameraPos(glm::vec3());
		std::shared_ptr<MenuControllerSystem> menuControl = g_engine.m_coordinator.GetSystem<MenuControllerSystem>();
		menuControl->InitPauseMenu();

		std::ostringstream ostrstream;
		ostrstream << "Resources/Levels/" << fileName;

		m_objectFactory->LoadLevel(ostrstream.str().c_str()); 
		std::vector<std::string>::iterator it = std::find(m_loadedLevels.begin(), m_loadedLevels.end(), std::string(fileName));
		if (it == m_loadedLevels.end())
			m_loadedLevels.push_back(std::string(fileName));

		/*Entity player = MAX_ENTITIES;
		
		if (fileName != "Level 20.json")
			player = m_objectFactory->Clone("FinalPlayer", true, false);

		if (player != MAX_ENTITIES)
			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(player))
			{
				std::string lastLevelLoaded = PLAYER_STATUS.GetLastLevel().data();
				//std::cout << lastLevelLoaded << std::endl;

				//For Tutorial Level to Hub
				if (lastLevelLoaded == "Level 10.json")
				{
					//Do as per normal
					trans->get().setPosition(PLAYER_STATUS.GetStartingPos());
				}
				//For Forest Level to Hub
				else if (lastLevelLoaded == "Level 15.json")
				{
					trans->get().setPosition(Vec2(1670.0f, -290.0f));
				}
				//For Mine Level to hub
				else if (lastLevelLoaded == "Level 17.json")
				{
					trans->get().setPosition(Vec2(-1050.0f, -400.0f));
				}
				//For Coral level to Hub
				else if (lastLevelLoaded == "Level 18.json")
				{
					trans->get().setPosition(Vec2(-40.0f, -400.0f));
				}
				//From Main Menu to Tutorial
				else if (lastLevelLoaded == "Level 20.json")
				{
					trans->get().setPosition(Vec2(-3532.46f, 756.053f));
				}

				Entity firstChild = g_engine.m_coordinator.GetHierarchyInfo(player).m_children.front();
				TransformComponent& childTrans = g_engine.m_coordinator.GetComponent<TransformComponent>(firstChild);
				std::cout << "FIRST CHILD TRANS " << childTrans.GetPosition().x << "," << childTrans.GetPosition().y << std::endl;

				HierarchyInfo& playerInfo = g_engine.m_coordinator.GetHierarchyInfo(player);
				for (auto entity : playerInfo.m_children)
				{
					if (auto child = g_engine.m_coordinator.TryGetComponent<ChildComponent>(entity))
					{
						child->get().SetGlobalDirty();
						child->get().ResetLocalDirty();
						g_engine.m_coordinator.ApplyParentChildCorrection(entity);
					}
				}
				std::cout << "FIRST CHILD TRANS AFTER UPDATE" << childTrans.GetPosition().x << "," << childTrans.GetPosition().y << std::endl;
			}*/

		//std::shared_ptr<MenuControllerSystem> menuControl = g_engine.m_coordinator.GetSystem<MenuControllerSystem>();
		//menuControl->InitPauseMenu();

		g_engine.m_coordinator.SystemInits();
		g_engine.m_coordinator.GetSystem<AudioSystem>()->TrueInit();
		//g_engine.m_coordinator.ResetEvents();

#if ENABLE_CORE_LOGGING
		auto end = Timer::instance().GetCurrTime();
		float duration = Timer::instance().CalculateDuration(start, end);
		std::stringstream ss;
		ss << "Time taken to load scene: " << duration << "s";
		RE_CORE_INFO(ss.str());
		CLEARSTRING(ss);
#endif
	}

	void SceneManager::SaveLevel(const char* fileName)
	{
		std::shared_ptr<MenuControllerSystem> menuControl = g_engine.m_coordinator.GetSystem<MenuControllerSystem>();
		std::ostringstream ostrstream;
		ostrstream << "Resources/Levels/" << fileName;
		int size = static_cast<int>(GetEntToSaveInLevel());

		if (m_objectFactory->CheckFileTooSmall(FILETYPE_LEVEL, size))
			if (size > 0)
				BasicIO::WriteLevelJsonFile(ostrstream.str().c_str(), size);
			else
				BasicIO::WriteLevelJsonFile(ostrstream.str().c_str(), 0);
		m_objectFactory->SaveLevel(ostrstream.str().c_str());
	}

	void SceneManager::SaveAndLoadLevel()
	{
		SaveLevel(m_currentFileName.c_str());
		LoadLevel(m_currentFileName.c_str());
	}

	void SceneManager::LoadTileset(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/Levels/" << fileName;
		m_objectFactory->LoadTileset(ostrstream.str().c_str());
	}

	void SceneManager::SaveTileset(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/Levels/" << fileName;
		m_objectFactory->SaveTileset(ostrstream.str().c_str());
	}

	void SceneManager::LoadArchetypes(const char* fileName)
	{
		m_objectFactory->LoadArchetypes(fileName);
	}

	void SceneManager::SaveArchetype(const char* fileName, Entity archetypeEntity)
	{
		m_objectFactory->SaveArchetype(fileName, archetypeEntity);
	}

	void SceneManager::SaveArchetypeList(const char* fileName)
	{
		m_objectFactory->SaveArchetypeList(fileName);
	}

	void SceneManager::AddToArchetypes(Entity archetypeEntity)
	{
		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
		auto it = activeObjects.begin();

		for (; it != activeObjects.end(); ++it)
		{
			if (*it == archetypeEntity)
				break;
		}

		if (it == activeObjects.end())
			return;

		HierarchyInfo& info = g_engine.m_coordinator.GetHierarchyInfo(*it);

		m_objectFactory->AddToArchetypes(
			info.m_objectName,
			g_engine.m_coordinator.GetEntityManager().GetSignature(*it),
			m_objectFactory->SerializeComponents(info),
			m_objectFactory->SerializeChildren(info));

		std::ostringstream ostrstream;
		ostrstream << "Resources/Archetypes/" << info.m_objectName << ".json";
		BasicIO::WriteArchetypeJsonFile(ostrstream.str());
		SaveArchetypeList("Resources/Archetypes/Archetypes.json");
	}

	void SceneManager::RemoveArchetype(std::string_view archetypeEntity)
	{
		auto it = m_objectFactory->GetArchetypeMap().begin();
		for (; it != m_objectFactory->GetArchetypeMap().end(); ++it)
		{
			if (it->first == archetypeEntity)
				break;
		}

		if (it == m_objectFactory->GetArchetypeMap().end())
			return;

		m_objectFactory->GetArchetypeMap().erase(it);
		m_objectFactory->SaveArchetypeList("Resources/Archetypes/Archetypes.json");
	}

	void SceneManager::UpdateArchetype(const char* archetypeName, Entity archetypeEntity)
	{
		SaveArchetype(archetypeName, archetypeEntity);
		//m_objectFactory->UpdateArchetype(archetypeName, archetypeEntity);
	}

	Entity SceneManager::Clone(const char* archetype, bool createHierarchy, bool hasSaveComponent)
	{
		return m_objectFactory->Clone(archetype, createHierarchy, hasSaveComponent);
		//MOVE_OBJECTFACTORY_TO_SCENEMANAGER;
	}

	std::vector<std::string> SceneManager::GetLoadedLevels() const
	{
		return m_loadedLevels;
	}

	void SceneManager::AddToLoadedLevels(std::string_view name)
	{
		m_loadedLevels.push_back(name.data());
	}

	void SceneManager::ResetMaxEntityInCurrentFile()
	{
		m_objectFactory->ResetMaxEntity();
	}

	void SceneManager::IncrementObjectIterator()
	{
		++m_objectIterator;
	}

	void SceneManager::ResetObjectIterator()
	{
		m_objectIterator = 0;
	}

	unsigned int SceneManager::GetObjectIterator() const
	{
		return m_objectIterator;
	}

	void SceneManager::IncrementSceneIterator()
	{
		++m_sceneIterator;
	}

	void SceneManager::ResetSceneIterator()
	{
		m_sceneIterator = 0;
	}

	unsigned int SceneManager::GetSceneIterator() const
	{
		return m_sceneIterator;
	}

	void SceneManager::ReloadLevel()
	{
		//ClearAllEntities is already called in LoadLevel now
		//ClearAllEntities();
		LoadLevel(getCurrentFileName().c_str());
		g_engine.m_coordinator.SetGameState(false);
		g_engine.m_coordinator.SetPauseState(false);
		g_engine.SetTimeScale(1.0f);
	}

	std::map<std::string, std::tuple<Signature, std::string, std::string>> SceneManager::GetArchetypeMap() const
	{
		return m_objectFactory->GetArchetypeMap();
	}

	void SceneManager::AddToActiveEntities(Entity newEnt, bool createHierarchy)
	{
		//Safety Check
		auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
		for (auto& iterator : activeObjects)
		{
			if (iterator == newEnt)
				return;
		}

		if (createHierarchy)
		{
			//g_engine.m_coordinator.GetActiveObjects().push_back(newEnt);
			std::ostringstream strstream;
			strstream << "Game Object " << m_objectIterator++;
			HierarchyInfo newInfo(newEnt, strstream.str());
			g_engine.m_coordinator.GetHierarchyInfo(newEnt) = newInfo;
		}
	}

	void SceneManager::DeleteActiveEntity(Entity ent)
	{
		auto& ActiveObjects = g_engine.m_coordinator.GetActiveObjects();
		for (auto object = ActiveObjects.begin(); object != ActiveObjects.end(); ++object)
		{
			if (*object == ent)
			{
				g_engine.m_coordinator.DestroyEntity(ent);
				ActiveObjects.erase(object);
				return;
			}
		}

		//RE_CORE_INFO("Object to delete not found");
	}

	Entity SceneManager::Create2DSprite()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.AddComponent<TransformComponent>(
			newEnt,
			TransformComponent(Vec2{ 0.0f, 0.0f }, Vec2{ 100.0f, 100.0f }, 0.0f));

		g_engine.m_coordinator.AddComponent<SaveComponent>(newEnt, SaveComponent());

		auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(newEnt);
		Sprite.Deserialize("Resources/Assets/DefaultSprite.png;1;1;1;1;1");

		std::ostringstream strstream;
		strstream << "Game Object " << m_objectIterator++;
		HierarchyInfo newInfo(newEnt, strstream.str());
		newInfo.m_selected = true;
		//g_engine.m_coordinator.GetActiveObjects().push_back(newEnt);
		g_engine.m_coordinator.GetHierarchyInfo(newEnt) = newInfo;
		return newEnt;
	}
	Entity SceneManager::CreateCamera()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.AddComponent<CameraComponent>(newEnt,CameraComponent());
		g_engine.m_coordinator.AddComponent<SaveComponent>(newEnt, SaveComponent());

		std::ostringstream strstream;
		strstream << "Camera " << m_cameraIterator++;
		HierarchyInfo newInfo(newEnt, strstream.str());
		newInfo.m_selected = true;
		//g_engine.m_coordinator.GetActiveObjects().push_back(newEnt);
		g_engine.m_coordinator.GetHierarchyInfo(newEnt) = newInfo;

		return newEnt;
	}
}