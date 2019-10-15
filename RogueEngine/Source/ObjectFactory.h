#pragma once
#include "REEngine.h"

class ObjectFactory
{

public:
	void LoadLevel(const char* fileName);
	void SaveLevel(const char* fileName);

	void Clone(Entity toClone);

	std::vector<Entity> GetActiveEntity() const;

private:

	std::vector<Entity> m_activeEntities;

};

//MACROS FOR OBJECT FACTORY
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARSTR(s) s.clear(); s.str("")
#define CLEARNSETSTR(s, i1, cmp) CLEARSTR(s); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
#define MAX_SAVE_ENTITY 8
#define MIN_SAVE_ENTITY 2