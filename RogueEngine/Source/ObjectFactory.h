#pragma once
#include "Main.h"

#define FILETYPE_LEVEL 0
#define FILETYPE_ARCHETYPE 1

namespace Rogue
{

	class ObjectFactory
	{

	public:
		void LoadLevel(const char* fileName);
		void SaveLevel(const char* fileName);

		void LoadArchetypes(const char* fileName);
		void SaveArchetypes(const char* fileName);

		void Clone(Entity toClone);
		void Clone(const char* archetype);

		bool CheckFileTooSmall(size_t type, size_t size);

	private:

		//Helper function
		void FactoryLoadComponent(Entity curEnt, Signature signature, std::string value);
		void SetArchetype(std::string archetypeName, std::string archetypeValue, Signature archetypeSignature);

		size_t m_maxEntityCount;
		size_t m_maxArcheTypeCount;
		std::map<std::string, std::string> m_archetypes;
		std::map<std::string, Signature> m_archetypeSignature;

	};

	//MACROS FOR OBJECT FACTORY
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARSTR(s) s.clear(); s.str("")
#define CLEARNSETSTR(s, i1, cmp) CLEARSTR(s); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
#define MAX_SAVE_ENTITY 8
#define MIN_SAVE_ENTITY 2

	//MACROS (HIERARCHY)
#define CREATE_HIERARCHY_OBJ(entityValue, ss)	CLEARSTR(ss);\
												ss << "Game Object " << g_engine.m_coordinator.GetSceneManager().GetIterator(); \
												stdstr = ss.str(); \
												HierarchyInfo newInfo(entityValue, stdstr); \
												g_engine.m_coordinator.GetEntityManager().m_getActiveObjects().push_back(newInfo); \
												g_engine.m_coordinator.GetSceneManager().IncrementIterator();

//cstr will go out of scope if you choose to do strstream.str().c_str()
//This is the proper (Non macro) way of setting the string
}