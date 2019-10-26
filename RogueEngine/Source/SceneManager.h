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

		Entity CreateDefaultEntity();
	};
}