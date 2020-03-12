/* Start Header ************************************************************************/
/*!
\file           ObjectFactory.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ObjectFactory

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once

#define FILETYPE_LEVEL 0
#define FILETYPE_ARCHETYPE 1
#define FILETYPE_LEVELFILE 2

namespace Rogue
{

	class ObjectFactory
	{

	public:
		void LoadLevel(const char* fileName);
		void SaveLevel(const char* fileName);
		void LoadTileset(const char* fileName);
		void SaveTileset(const char* fileName);

		void LoadArchetypes(const char* fileName);
		void SaveArchetypeList(const char* fileName = "Resources/Archetypes/Archetypes.json");
		void SaveArchetype(std::string_view file, Entity archetypeEntity);
		void AddToArchetypes(std::string_view archetype, Signature signature, std::string_view toDeserialize, std::string_view children);
		void UpdateArchetype(const char* archetype, Entity entityToReplace);

		void LoadLevelFiles(const char* fileName);
		void SaveLevelFiles(const char* fileName);

		Entity Clone(const char* archetype, bool createHierarchy = true, bool hasSaveComponent = true);

		bool CheckFileTooSmall(size_t type, size_t size);
		void ResetMaxEntity();
		void AssignParentChild(HierarchyInfo& parent, HierarchyInfo& child);

		std::string SerializeComponents(HierarchyInfo& entityHierarchy);
		std::string SerializeChildren(HierarchyInfo& entityHierarchy);

		std::map<std::string, std::tuple<Signature, std::string, std::string>> GetArchetypeMap() const;

	private:

		//Helper function
		void FactoryLoadComponent(Entity curEnt, Signature signature, std::string_view value, bool createSaveComponent = true);

		size_t m_maxEntityCount; //Represents max entity size of FILE (not level)
		size_t m_maxArchetypeCount;
		size_t m_maxFileCount;
		std::map<std::string, std::tuple<Signature, std::string, std::string>> m_archetypes;

		Entity m_firstLoadedEntity;
		Entity m_tilesetEntity = MAX_ENTITIES;
	};

	//MACROS FOR OBJECT FACTORY
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARSTR(s) s.clear(); s.str("")
#define CLEARNSETSTR(s, i1, cmp) CLEARSTR(s); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
#define MAX_SAVE_ENTITY 8
#define MIN_SAVE_ENTITY 2

	//MACROS (HIERARCHY)
#define CREATE_HIERARCHY_OBJ(entityValue, name, tag, archetypeName, parent)		HierarchyInfo newInfo(entityValue, name, tag, archetypeName, parent); \
																				g_engine.m_coordinator.GetHierarchyInfo(entityValue) = newInfo; \
																				SceneManager::instance().IncrementObjectIterator();

//cstr will go out of scope if you choose to do strstream.str().c_str()
//This is the proper (Non macro) way of setting the string
}