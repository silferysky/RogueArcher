/* Start Header ************************************************************************/
/*!
\file           TileMapSystem.cpp
\project        Exale
\author         Loh Kai Yi,kaiyi.loh,390002918 (100%)
\par            kaiyi.loh\@digipen.edu
\date           4 April,2020
\brief          This file contains the functions for TileMapSystem

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "TileMapSystem.h"
#include "EventDispatcher.h"
#include "TileMapComponent.h"

namespace Rogue
{
	void TileMapSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_TILEMAPSYSTEM, TileMapSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TileMapComponent>());

		g_engine.m_coordinator.SetSystemSignature<TileMapSystem>(signature);
	}

	void TileMapSystem::Update()
	{
	}

	void TileMapSystem::Shutdown()
	{
	}

	void TileMapSystem::Receive(Event& ev)
	{
	}
}