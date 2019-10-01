#pragma once
#include "REEngine.h"
#include "FileIO.h"
#include "BasicIO.h"
#include "Config.h"

class ObjectFactory
{
	RESerialiser m_Serialiser;

public:
	void SaveLevel(const char* fileName);
	void LoadLevel(const char* fileName);

	std::vector<Entity> GetActiveEntity() const;

private:
	std::vector<Entity> m_activeEntities;
};

//MACROS FOR OBJECT FACTORY
#define SETSTRING(str, i1, cmp, i2) str << "e" << i1 << cmp << i2
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARNSETSTR(s, i1, cmp, i2) s.clear(); s.str(""); SETSTRING(s, i1, cmp, i2); SETSSTOSTR(s)
#define MAX_SAVE_ENTITY 8
#define MIN_SAVE_ENTITY 2
#define SIGNATURE_SPRITECOMPONENT 1
#define SIGNATURE_TRANSFORMCOMPONENT 4