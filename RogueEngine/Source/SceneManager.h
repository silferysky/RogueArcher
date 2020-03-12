/* Start Header ************************************************************************/
/*!
\file           SceneManager.h
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
#pragma once
#include "Types.h"
#include "Vector2D.h"

namespace Rogue
{
	class ObjectFactory;
	class EditorCreateObjectCommand;
	class SceneManager
	{
		std::unique_ptr<ObjectFactory> m_objectFactory;
		std::vector<std::string> m_loadedLevels;
		std::string m_currentFileName;
		unsigned int m_objectIterator = 0;
		unsigned int m_sceneIterator = 0;
		unsigned int m_cameraIterator = 0;
		std::unique_ptr<EditorCreateObjectCommand> m_command;

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

		size_t GetEntToSaveInLevel() const;
		void LoadLevel(const std::string& fileName);
		void SaveLevel(const char* fileName);
		void SaveAndLoadLevel();
		void LoadTileset(const char* fileName);
		void SaveTileset(const char* fileName);

		void LoadArchetypes(const char* fileName);
		void SaveArchetype(const char* fileName, Entity archetypeEntity);
		void SaveArchetypeList(const char* fileName);
		void AddToArchetypes(Entity archetypeEntity);
		void RemoveArchetype(std::string_view archetypeEntity);
		void UpdateArchetype(const char* archetypeName, Entity archetypeEntity);

		Entity Clone(const char* archetype, bool createHierarchy = true, bool hasSaveComponent = true);


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

		std::map<std::string, std::tuple<Signature, std::string, std::string>> GetArchetypeMap() const;

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