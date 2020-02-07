/* Start Header ************************************************************************/
/*!
\file           AnimationSystem.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for AnimationSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
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
			
			if (animate.getIsNotReversed())
				UpdateTexture(&animate, &sprite);
			else
				UpdateTextureReversed(&animate, &sprite);
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

		double currentFrame = (double)animate->getCurrentFrame();
		int totalFrames = animate->getFrames();

		// reset the frame number
		if (currentFrame >= animate->getFrames())
		{
			if (!animate->getIsLooping())
			{
				animate->setIsAnimating(false);
				currentFrame = animate->getFrames() - 1;
			}
			else
				currentFrame = 0;
			animate->setCurrentFrame(static_cast<int>(currentFrame));
		}

		double min = currentFrame / totalFrames;
		double max = ++currentFrame / totalFrames;

		sprite->setTexCoordMin(static_cast<float>(min));
		sprite->setTexCoordMax(static_cast<float>(max));

		if (animate->getIsAnimating())
			animate->setCurrentFrame(static_cast<int>(currentFrame));
	}

	void AnimationSystem::UpdateTextureReversed(AnimationComponent* animate, SpriteComponent* sprite)
	{
		animate->updateTimer();

		// have not yet reached next frame
		if (animate->getTimer() < animate->getSecondsPerFrame())
			return;

		animate->setTimer(0);

		double currentFrame = (double)animate->getCurrentFrame();
		int totalFrames = animate->getFrames();

		// reset the frame number
		if (currentFrame <= 0)
		{
			if (!animate->getIsLooping())
			{
				animate->setIsAnimating(false);
				currentFrame = 1;
			}
			else
				currentFrame = animate->getFrames();
			animate->setCurrentFrame(static_cast<int>(currentFrame));
		}

		double min = (currentFrame - 1) / totalFrames;
		double max = currentFrame-- / totalFrames;

		sprite->setTexCoordMin(static_cast<float>(min));
		sprite->setTexCoordMax(static_cast<float>(max));

		if (animate->getIsAnimating())
			animate->setCurrentFrame(static_cast<int>(currentFrame));
	}

	void AnimationSystem::InitAnimations()
	{
		for (auto entity : m_entities)
		{
			auto& animate = g_engine.m_coordinator.GetComponent<AnimationComponent>(entity);
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);

			if (animate.getIsNotReversed())
			{
				sprite.setTexCoordMin(0.0f);
				sprite.setTexCoordMax(1.0f / animate.getFrames());
				animate.setCurrentFrame(animate.getStartFrame());
			}
			else
			{
				sprite.setTexCoordMin(animate.getFrames() - 1.0f / animate.getFrames());
				sprite.setTexCoordMax(1.0f);
				animate.setCurrentFrame(animate.getFrames());
			}
		}
	}

	void AnimationSystem::Shutdown()
	{}

	void AnimationSystem::Receive(Event* ev)
	{}
}