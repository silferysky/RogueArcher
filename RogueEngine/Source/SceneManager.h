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
		std::vector<std::string> m_loadedLevels;
		std::string m_currentFileName;
		unsigned int m_objectIterator = 0;
		unsigned int m_sceneIterator = 0;
		unsigned int m_cameraIterator = 0;

	public:
		SceneManager();
		~SceneManager();

		std::string getCurrentFileName() const;
		void setCurrentFileName(std::string curFileName);

		void ClearActiveEntities();
		void ClearAllEntities();

		void LoadLevelFiles(const char* fileName);
		void SaveLevelFiles(const char* fileName);

		void LoadLevel(const char* fileName);
		void SaveLevel(const char* fileName);
		void SaveAndLoadLevel();

		void LoadArchetypes(const char* fileName);
		void SaveArchetypes(const char* fileName);

		void Clone(Entity toClone);
		void Clone(const char* archetype);

		std::vector<std::string> GetLoadedLevels() const;
		void AddToLoadedLevels(std::string name);

		void IncrementObjectIterator();
		void ResetObjectIterator();
		unsigned int GetObjectIterator() const;

		void IncrementSceneIterator();
		void ResetSceneIterator();
		unsigned int GetSceneIterator() const;

		//For other systems to add entites here
		void AddToActiveEntities(Entity ent);

		Entity Create2DSprite();
		Entity CreateCamera();
	};

/*#define MOVE_OBJECTFACTORY_TO_SCENEMANAGER	auto& entityManager = g_engine.m_coordinator.GetEntityManager(); \
											std::vector<Entity> entityVector = m_objectFactory->GetRecentEntities(); \
											for (auto& entity : entityVector) \
												AddToActiveEntities(entity);*/
}