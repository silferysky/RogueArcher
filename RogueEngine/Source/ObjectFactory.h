#pragma once
#include "Main.h"

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

		void LoadArchetypes(const char* fileName);
		void SaveArchetypeList(const char* fileName);
		void SaveArchetype(std::string_view file);
		void AddToArchetypes(std::string_view archetype, Signature signature, std::string_view toDeserialize);

		void LoadLevelFiles(const char* fileName);
		void SaveLevelFiles(const char* fileName);

		void Clone(Entity toClone);
		Entity Clone(const char* archetype);

		bool CheckFileTooSmall(size_t type, size_t size);
		void ResetMaxEntity();

		std::string SerializeComponents(HierarchyInfo& entityHierarchy);

		std::map<std::string, std::pair<Signature, std::string>> GetArchetypeMap() const;

	private:

		//Helper function
		void FactoryLoadComponent(Entity curEnt, Signature signature, std::string_view value);

		size_t m_maxEntityCount; //Represents max entity size of FILE (not level)
		size_t m_maxArchetypeCount;
		size_t m_maxFileCount;
		std::map<std::string, std::pair<Signature, std::string>> m_archetypes;

	};

	//MACROS FOR OBJECT FACTORY
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARSTR(s) s.clear(); s.str("")
#define CLEARNSETSTR(s, i1, cmp) CLEARSTR(s); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
#define MAX_SAVE_ENTITY 8
#define MIN_SAVE_ENTITY 2

	//MACROS (HIERARCHY)
#define CREATE_HIERARCHY_OBJ_TAG(entityValue, name, tag)	HierarchyInfo newInfo(entityValue, name, tag); \
															g_engine.m_coordinator.GetActiveObjects().push_back(entityValue); \
															g_engine.m_coordinator.GetHierarchyInfo(entityValue) = newInfo; \
															SceneManager::instance().IncrementObjectIterator();
	
#define CREATE_HIERARCHY_OBJ(entityValue, name)				HierarchyInfo newInfo(entityValue, name); \
															g_engine.m_coordinator.GetActiveObjects().push_back(entityValue); \
															g_engine.m_coordinator.GetHierarchyInfo(entityValue) = newInfo; \
															SceneManager::instance().IncrementObjectIterator();

//cstr will go out of scope if you choose to do strstream.str().c_str()
//This is the proper (Non macro) way of setting the string
}