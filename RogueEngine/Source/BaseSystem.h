/* Start Header ************************************************************************/
/*!
\file           BaseSystem.h
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for BaseSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

		const std::set<Entity>& GetEntitySet() const
		{
			return m_entities;
		}
	};
}