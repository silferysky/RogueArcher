#pragma once
#include <set>
#include "SystemList.h"
#include "Types.h"

namespace Rogue
{
	class System
	{
	public:
		std::set<Entity> m_entities;
		SystemID m_systemID = SystemID::id_LASTSYS;

		System() = default;
		System(SystemID id)
			: m_systemID{ id } {}
		virtual ~System() = default;
		virtual void init() = 0;
		virtual void update() = 0;
	};
}
