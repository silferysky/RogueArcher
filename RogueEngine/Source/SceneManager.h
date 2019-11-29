#pragma once
#include "Types.h"
#include "Vector2D.h"

namespace Rogue
{
	class ObjectFactory;
	class SceneManager
	{
		std::unique_ptr<ObjectFactory> m_objectFactory;
		std::vector<std::string> m_loadedLevels;
		std::string m_currentFileName;
		Vec2 m_levelCameraMin{};
		Vec2 m_levelCameraMax{};
		float m_levelCameraZoom = 0.0f;
		unsigned int m_objectIterator = 0;
		unsigned int m_sceneIterator = 0;
		unsigned int m_cameraIterator = 0;

	public:
		static SceneManager& instance()
		{
			static SceneManager instance;
			return instance;
		}

		SceneManager();
		~SceneManager();

		std::string getCurrentFileName() const;
		void setCurrentFileName(std::string_view curFileName);

		void ClearActiveEntities();
		void ClearAllEntities();

		void LoadLevelFiles(const char* fileName);
		void SaveLevelFiles(const char* fileName);

		void LoadLevel(const std::string& fileName);
		void SaveLevel(const char* fileName);
		void SaveAndLoadLevel();

		void LoadArchetypes(const char* fileName);
		void SaveArchetype(const char* fileName);
		void SaveArchetypeList(const char* fileName);
		void AddToArchetypes(Entity archetypeEntity);
		void RemoveArchetype(std::string_view archetypeEntity);

		void Clone(Entity toClone);
		Entity Clone(const char* archetype, bool createHierarchy = true);

		Vec2 GetLevelCameraMin() const;
		Vec2 GetLevelCameraMax() const;
		float GetLevelCameraZoom() const;

		void SetLevelCameraMin(const Vec2& cameraMin);
		void SetLevelCameraMax(const Vec2& cameraMax);
		void SetLevelCameraZoom(const float& cameraZoom);


		std::vector<std::string> GetLoadedLevels() const;
		void AddToLoadedLevels(std::string_view name);
		void ResetMaxEntityInCurrentFile();

		void IncrementObjectIterator();
		void ResetObjectIterator();
		unsigned int GetObjectIterator() const;

		void IncrementSceneIterator();
		void ResetSceneIterator();
		unsigned int GetSceneIterator() const;

		void ReloadLevel();

		std::map<std::string, std::pair<Signature, std::string>> GetArchetypeMap() const;

		//For other systems to add entites here
		void AddToActiveEntities(Entity ent, bool createHierarchy = true);
		void DeleteActiveEntity(Entity ent);

		Entity Create2DSprite();
		Entity CreateCamera();
	};

/*#define MOVE_OBJECTFACTORY_TO_SCENEMANAGER	auto& entityManager = g_engine.m_coordinator.GetEntityManager(); \
											std::vector<Entity> entityVector = m_objectFactory->GetRecentEntities(); \
											for (auto& entity : entityVector) \
												AddToActiveEntities(entity);*/
}