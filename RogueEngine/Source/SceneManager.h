#pragma once
#include <vector>
#include <memory>
#include "Types.h"

namespace Rogue
{
	class ObjectFactory;
	class SceneManager
	{
		std::unique_ptr<ObjectFactory> m_objectFactory;
		std::string m_currentFileName;
		unsigned int iterator = 0;

	public:
		SceneManager();
		~SceneManager();

		std::string getCurrentFileName() const;
		void setCurrentFileName(std::string curFileName);

		void ClearActiveEntities();
		void ClearAllEntities();

		void LoadLevel(const char* fileName);
		void SaveLevel(const char* fileName);

		void LoadArchetypes(const char* fileName);
		void SaveArchetypes(const char* fileName);

		void Clone(Entity toClone);
		void Clone(const char* archetype);

		void IncrementIterator();
		unsigned int GetIterator() const;

		//For other systems to add entites here
		void AddToActiveEntities(Entity ent);

		Entity CreateDefaultEntity();
	};

#define MOVE_OBJECTFACTORY_TO_SCENEMANAGER	auto& entityManager = g_engine.m_coordinator.GetEntityManager(); \
											std::vector<Entity> entityVector = m_objectFactory->GetRecentEntities(); \
											for (auto& entity : entityVector) \
												AddToActiveEntities(entity);
}