#include "AnimationSystem.h"
#include "Main.h"

namespace Rogue
{
	AnimationSystem::AnimationSystem()
		: System(SystemID::id_ANIMATIONSYSTEM)
	{}

	void AnimationSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_ANIMATIONSYSTEM, AnimationSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<SpriteComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<AnimationComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<AnimationSystem>(signature);
	}

	void AnimationSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Animation System");

		// For all entities
		for (auto entity : m_entities)
		{
			auto& animate = g_engine.m_coordinator.GetComponent<AnimationComponent>(entity);

			if (animate.getIsAnimating() == false)
				continue;

			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
			
			//TODO: change texture coordinates
		}

		g_engine.m_coordinator.EndTimeSystem("Animation System");
	}

	void AnimationSystem::UpdateTexture(AnimationComponent* animComp)
	{
	}

	void AnimationSystem::Shutdown()
	{}

	void AnimationSystem::Receive(Event* ev)
	{}
}