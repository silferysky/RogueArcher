#pragma once
#include "SystemList.h"
#include "Types.h"

namespace Rogue
{
	class System
	{
	public:
		std::set<Entity> m_entities;
		SystemID m_systemID = SystemID::id_LASTSYS;

		System(SystemID id) :
			m_entities{ std::set<Entity>{} }, m_systemID{ id }
		{}

		virtual ~System() = default;
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};
}