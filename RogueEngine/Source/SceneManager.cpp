#include "SceneManager.h"
#include "ObjectFactory.h"

namespace Rogue
{
	SceneManager::SceneManager()
	:	m_objectFactory {std::make_unique<ObjectFactory>()},
		m_activeEntities {std::vector<Entity>()},
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
		m_activeEntities.clear();
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
		for (auto iterator = m_activeEntities.begin(); iterator != m_activeEntities.end(); ++iterator)
		{
			if (*iterator == ent)
				return;
		}

		m_activeEntities.push_back(ent);
	}

	Entity SceneManager::CreateDefaultEntity()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.CreateComponent<TransformComponent>(newEnt);
		m_activeEntities.push_back(newEnt);
		return newEnt;
	}
}