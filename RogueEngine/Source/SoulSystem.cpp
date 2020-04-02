/* Start Header ************************************************************************/
/*!
\file           SoulSystem.cpp
\project        Exale
\author         Lim Jia Hao, l.jiahao, 390006518 (100%)
\par            l.jiahao\@digipen.edu
\date           3 April,2020
\brief          This file contains the functions for SoulSystem

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"

#include "SoulSystem.h"
#include "EventDispatcher.h"

namespace Rogue
{
	SoulSystem::SoulSystem()
		: System(SystemID::id_SOULSYSTEM)
	{}

	void SoulSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_SOULSYSTEM, SoulSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<SoulComponent>());

		g_engine.m_coordinator.SetSystemSignature<SoulSystem>(signature);
	}

	void SoulSystem::TrueInit()
	{
		// Current level is set in Coordinator::SystemInits()

		// If level successfully registered
		if (PLAYER_STATUS.RegisterLevel(PLAYER_STATUS.GetCurrLevel()))
		{
			// Add all souls into vector for this level
			for (Entity entity : m_entities)
			{
				PLAYER_STATUS.AddSoul(entity);
			}
		}
		else // If level has already been registered
			PLAYER_STATUS.RemoveCollectedSouls();
	}

	void SoulSystem::Update()
	{
		//std::cout << "Total Souls in Level: " << PLAYER_STATUS.GetTotalSoulsInLevel()
		//	<< " Collected Souls in Level: " << PLAYER_STATUS.GetCollectedSoulsInLevel()
		//	<< " Current Level: " << PLAYER_STATUS.GetCurrLevel()
		//	<< std::endl;

		//for (Entity entity : m_entities)
		//{
		//	SoulComponent& soul = g_engine.m_coordinator.GetComponent<SoulComponent>(entity);
		//}
	}

	void SoulSystem::Shutdown()
	{}

	void SoulSystem::Receive(Event & ev)
	{
	}
}