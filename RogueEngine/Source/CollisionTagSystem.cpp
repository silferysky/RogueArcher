#include "CollisionTagSystem.h"
#include "Main.h"

namespace Rogue
{
	CollisionTagSystem::CollisionTagSystem()
		: System(SystemID::id_COLLISIONTAGSYSTEM)
	{}

	void CollisionTagSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_COLLISIONTAGSYSTEM, CollisionTagSystem::Receive);

		// Add components to signature.
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<RigidbodyComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set Collision system signature.
		g_engine.m_coordinator.SetSystemSignature<CollisionTagSystem>(signature);
	}

	void CollisionTagSystem::Update()
	{
	}

	void CollisionTagSystem::Shutdown()
	{
	}

	void CollisionTagSystem::Receive(Event* ev)
	{
	}
}