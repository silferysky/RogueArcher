#pragma once
#include <set>
#include "SystemList.h"
#include "Types.h"

class System
{
public:
	std::set<Entity> m_entities;
	SystemID m_systemID = SystemID::id_LASTSYS;

	System() = default;
	virtual void init() = 0;
	virtual void update() = 0;
	virtual ~System() = default;
};