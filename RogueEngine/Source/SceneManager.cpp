#include "Precompiled.h"
#include "SceneManager.h"
#include "ObjectFactory.h"
#include "EditorHierarchyInfo.h"
#include <sstream>
#include "BasicIO.h"

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
		ClearActiveEntities();
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
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().clear();
	}

	void SceneManager::ClearAllEntities()
	{
		g_engine.m_coordinator.GetSystem<AudioSystem>()->ShutdownSounds();
		g_engine.m_coordinator.DestroyAllEntity();
		ClearActiveEntities();
	}

	void SceneManager::LoadLevelFiles(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/" << fileName;
		m_objectFactory->LoadLevelFiles(ostrstream.str().c_str());
		m_sceneIterator = static_cast<unsigned int>(m_loadedLevels.size());
	}

	void SceneManager::SaveLevelFiles(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/" << fileName;
		m_objectFactory->SaveLevelFiles(ostrstream.str().c_str());
	}

	void SceneManager::LoadLevel(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/" << fileName;

		m_objectFactory->LoadLevel(ostrstream.str().c_str()); 
		std::vector<std::string>::iterator it = std::find(m_loadedLevels.begin(), m_loadedLevels.end(), std::string(fileName));
		if (it == m_loadedLevels.end())
			m_loadedLevels.push_back(std::string(fileName));

		g_engine.m_coordinator.SystemInits();
	}

	void SceneManager::SaveLevel(const char* fileName)
	{
		std::ostringstream ostrstream;
		ostrstream << "Resources/" << fileName;
		if (m_objectFactory->CheckFileTooSmall(FILETYPE_LEVEL, g_engine.m_coordinator.GetActiveObjects().size()))
			if (g_engine.m_coordinator.GetActiveObjects().size() != 0)
				BasicIO::WriteLevelJsonFile(ostrstream.str().c_str(), g_engine.m_coordinator.GetActiveObjects().size() - 1);
			else
				BasicIO::WriteLevelJsonFile(ostrstream.str().c_str(), 0);
		m_objectFactory->SaveLevel(ostrstream.str().c_str());
	}

	void SceneManager::SaveAndLoadLevel()
	{
		SaveLevel(m_currentFileName.c_str());
		ClearAllEntities();
		LoadLevel(m_currentFileName.c_str());
	}

	void SceneManager::LoadArchetypes(const char* fileName)
	{
		m_objectFactory->LoadArchetypes(fileName);
	}

	void SceneManager::SaveArchetype(const char* fileName)
	{
		m_objectFactory->SaveArchetype(fileName);
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
			if (it->m_Entity == archetypeEntity)
				break;
		}

		if (it == activeObjects.end())
			return;

		m_objectFactory->AddToArchetypes(
			it->m_objectName,
			g_engine.m_coordinator.GetEntityManager().GetSignature(it->m_Entity),
			m_objectFactory->SerializeComponents(*it));

		std::ostringstream ostrstream;
		ostrstream << "Resources/" << it->m_objectName << ".json";
		BasicIO::WriteArchetypeJsonFile(ostrstream.str());
		SaveArchetypeList("Resources/Archetypes.json");
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
		m_objectFactory->SaveArchetypeList("Resources/Archetypes.json");
	}

	void SceneManager::Clone(Entity toClone)
	{
		m_objectFactory->Clone(toClone);
		//MOVE_OBJECTFACTORY_TO_SCENEMANAGER;
	}

	void SceneManager::Clone(const char* archetype)
	{
		m_objectFactory->Clone(archetype);
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

	std::map<std::string, std::pair<Signature, std::string>> SceneManager::GetArchetypeMap() const
	{
		return m_objectFactory->GetArchetypeMap();
	}

	void SceneManager::AddToActiveEntities(Entity newEnt)
	{
		//Safety Check
		auto& activeObjects = g_engine.m_coordinator.GetEntityManager().m_getActiveObjects();
		for (auto& iterator : activeObjects)
		{
			if (iterator.m_Entity == newEnt)
				return;
		}

		HierarchyInfo newInfo{};
		newInfo.m_Entity = newEnt;
		std::ostringstream strstream;
		std::string sstr;
		strstream << "Game Object " << m_objectIterator++;
		sstr = strstream.str();
		newInfo.m_objectName = sstr;
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo);
	}

	void SceneManager::DeleteActiveEntity(Entity ent)
	{
		auto& ActiveObjects = g_engine.m_coordinator.GetActiveObjects();
		for (auto object = ActiveObjects.begin(); object != ActiveObjects.end(); ++object)
		{
			if (object->m_Entity == ent)
			{
				g_engine.m_coordinator.DestroyEntity(ent);
				ActiveObjects.erase(object);
				return;
			}
		}

		RE_CORE_INFO("Object to delete not found");
	}

	Entity SceneManager::Create2DSprite()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.AddComponent<TransformComponent>(
			newEnt,
			TransformComponent(Vec2{ 0.0f, 0.0f }, Vec2{ 100.0f, 100.0f }, 0.0f));

		auto& Sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(newEnt);
		Sprite.Deserialize("Resources/Assets/DefaultSprite.png;1;1;1;1;1");

		HierarchyInfo newInfo{};
		newInfo.m_selected = true;
		newInfo.m_Entity = newEnt;
		std::ostringstream strstream;
		std::string sstr;
		strstream << "Game Object " << m_objectIterator++;
		sstr = strstream.str();
		newInfo.m_objectName = sstr;
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo);

		return newEnt;
	}
	Entity SceneManager::CreateCamera()
	{
		Entity m_newentity = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.AddComponent<CameraComponent>(m_newentity,CameraComponent());

		HierarchyInfo newInfo{};
		newInfo.m_Entity = m_newentity;
		std::ostringstream strstream;
		std::string sstr;
		strstream << "Camera " << m_cameraIterator++;
		sstr = strstream.str();
		newInfo.m_objectName = sstr;
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo);

		return m_newentity;
	}
}