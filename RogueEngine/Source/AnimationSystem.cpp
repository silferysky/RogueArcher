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
			
			UpdateTexture(&animate, &sprite);
		}

		g_engine.m_coordinator.EndTimeSystem("Animation System");
	}

	void AnimationSystem::UpdateTexture(AnimationComponent* animate, SpriteComponent* sprite)
	{
		animate->updateTimer();

		// have not yet reached next frame
		if (animate->getTimer() < animate->getSecondsPerFrame())
			return;

		animate->setTimer(0);

		double currentFrame = animate->getCurrentFrame();
		int totalFrames = animate->getFrames();

		// reset the frame number
		if (currentFrame >= animate->getFrames())
		{
			currentFrame = 0;
			animate->setCurrentFrame(currentFrame);
			if (!animate->getIsLooping())
				animate->setIsAnimating(false);
		}

		double min = currentFrame / totalFrames;
		double max = ++currentFrame / totalFrames;

		sprite->setTexCoordMin(min);
		sprite->setTexCoordMax(max);

		if (animate->getIsAnimating())
			animate->setCurrentFrame(currentFrame);
	}

	void AnimationSystem::Shutdown()
	{}

	void AnimationSystem::Receive(Event* ev)
	{}
}