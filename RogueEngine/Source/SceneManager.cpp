#include "SceneManager.h"
#include "ObjectFactory.h"

namespace Rogue
{
	SceneManager::SceneManager()
	:	m_objectFactory {std::make_unique<ObjectFactory>()},
		m_activeEntities {std::vector<Entity>()},
		m_currentFileName {std::string()}
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
	}

	void SceneManager::LoadLevel(const char* fileName)
	{
		m_activeEntities.clear();
		m_objectFactory->LoadLevel(fileName);
		//Note, start at begin since m_activeEntites is empty
		std::vector<Entity> entityVector = m_objectFactory->GetRecentEntities();
		m_activeEntities.insert(m_activeEntities.begin(), entityVector.begin(), entityVector.end());
		m_objectFactory->ClearRecentEntities();
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
	}

	void SceneManager::Clone(const char* archetype)
	{
		m_objectFactory->Clone(archetype);
	}

	Entity SceneManager::CreateDefaultEntity()
	{
		Entity newEnt = g_engine.m_coordinator.CreateEntity();
		g_engine.m_coordinator.CreateComponent<TransformComponent>(newEnt);
		return newEnt;
	}
}