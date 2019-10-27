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
		std::vector<Entity> m_activeEntities;
		std::string m_currentFileName;

	public:
		SceneManager();
		~SceneManager();

		std::string getCurrentFileName() const;
		void setCurrentFileName(std::string curFileName);

		void ClearActiveEntities();

		void LoadLevel(const char* fileName);
		void SaveLevel(const char* fileName);

		void LoadArchetypes(const char* fileName);
		void SaveArchetypes(const char* fileName);

		void Clone(Entity toClone);
		void Clone(const char* archetype);

		//For other systems to add entites here
		void AddToActiveEntities(Entity ent);

		Entity CreateDefaultEntity();
	};

#define MOVE_OBJECTFACTORY_TO_SCENEMANAGER	std::vector<Entity> entityVector = m_objectFactory->GetRecentEntities(); \
											m_activeEntities.insert(m_activeEntities.begin(), entityVector.begin(), entityVector.end()); \
											m_objectFactory->ClearRecentEntities()
}