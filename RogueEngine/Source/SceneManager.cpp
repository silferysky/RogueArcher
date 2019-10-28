#include "SceneManager.h"
#include "ObjectFactory.h"
#include "EditorHierarchyInfo.h"

namespace Rogue
{
	SceneManager::SceneManager()
	:	m_objectFactory {std::make_unique<ObjectFactory>()},
		m_currentFileName { "Resources/Level 1.json"}
	{
	}

	SceneManager::~SceneManager()
	{
		ClearActiveEntities();
	}

	std::string SceneManager::getCurrentFileName() const
	{
		return m_currentFileName;
	}

	void SceneManager::setCurrentFileName(std::string curFileName)
	{
		m_currentFileName = curFileName;
	}

	void SceneManager::ClearActiveEntities()
	{
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().clear();
	}

	void SceneManager::ClearAllEntities()
	{
		ClearActiveEntities();
		g_engine.m_coordinator.DestroyAllEntity();
	}

	void SceneManager::LoadLevel(const char* fileName)
	{
		m_objectFactory->LoadLevel(fileName);
		MOVE_OBJECTFACTORY_TO_SCENEMANAGER;
	}

	void SceneManager::SaveLevel(const char* fileName)
	{
		m_objectFactory->SaveLevel(fileName);
	}

	void SceneManager::LoadArchetypes(const char* fileName)
	{
		m_objectFactory->LoadArchetypes(fileName);
	}

	void SceneManager::SaveArchetypes(const char* fileName)
	{
		m_objectFactory->SaveArchetypes(fileName);
	}

	void SceneManager::Clone(Entity toClone)
	{
		m_objectFactory->Clone(toClone);
		MOVE_OBJECTFACTORY_TO_SCENEMANAGER;
	}

	void SceneManager::Clone(const char* archetype)
	{
		m_objectFactory->Clone(archetype);
		MOVE_OBJECTFACTORY_TO_SCENEMANAGER;
	}

	void SceneManager::AddToActiveEntities(Entity ent)
	{
		//Safety Check
		auto& activeObjects = g_engine.m_coordinator.GetEntityManager().m_getActiveObjects();
		for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
		{
			if (iterator->m_Entity == ent)
				return;
		}

		HierarchyInfo newInfo{};
		newInfo.m_Entity = ent;
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo);
	}

	Entity SceneManager::CreateDefaultEntity()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.CreateComponent<TransformComponent>(newEnt);
		HierarchyInfo newInfo{};
		newInfo.m_Entity = newEnt;
		g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo);
		return newEnt;
	}
}