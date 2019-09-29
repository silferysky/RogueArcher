#pragma once
#include "Main.h"
#include "RogueEngine.h"
#include "FileIO.h"
#include "ComponentList.h"

class ObjectFactory
{
	RESerialiser m_Serialiser;

public:
	void SaveLevel(const char* fileName);
	void LoadLevel(const char* fileName);

private:
	ComponentType GetCmpType(int index) const;
};

//MACROS FOR OBJECT FACTORY
#define CLEARSTRING(s) s.clear(); s.str("")
#define SETSTRING(str, i1, cmp, i2) str << "e" << i1 << cmp << i2
#define SETSSTOSTR(ss) stdstr = ss.str(); cstr = stdstr.c_str()
#define CLEARNSETSTR(s, i1, cmp, i2) s.clear(); s.str(""); SETSTRING(s, i1, cmp, i2); SETSSTOSTR(s)