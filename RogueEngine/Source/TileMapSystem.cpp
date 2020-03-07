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