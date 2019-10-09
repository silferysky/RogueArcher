#pragma once
#include "Types.h"
#include <vector>

class ObjectFactory
{

public:
	void SaveLevel(const char* fileName);
	void LoadLevel(const char* fileName);

	std::vector<Entity> GetActiveEntity() const;

private:
	std::vector<Entity> m_activeEntities;
};

//MACROS FOR OBJECT FACTORY
#define SETSTRING(str, i1, cmp) str << "Entity" << i1 << cmp
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARNSETSTR(s, i1, cmp) s.clear(); s.str(""); SETSTRING(s, i1, cmp); SETSSTOSTR(s)
#define MAX_SAVE_ENTITY 8
#define MIN_SAVE_ENTITY 2