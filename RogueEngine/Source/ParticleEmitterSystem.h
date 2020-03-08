/* Start Header ************************************************************************/
/*!
\file           ParticleEmitterSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ParticleEmitterSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "EventListener.h"
#include "BaseSystem.h"

static float RandFloat()
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
}

static float RandFloat(const float& limit)
{
	return static_cast <float> (rand()) / static_cast <float> (RAND_MAX / limit);
}

static float RandFloat(const float& lowerLimit, const float& upperLimit)
{
	return lowerLimit + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (upperLimit - lowerLimit)));
}

namespace Rogue
{
	class ParticleEmitterSystem
		: public System, public EventListener
	{
		Entity unitParticle;
	public:
		ParticleEmitterSystem();
		~ParticleEmitterSystem() = default;

		//Basic System 
		void Init() override;
		void Update() override;
		void Receive(Event& ev) override;
		void Shutdown() override;

		void GenerateParticles(const Entity& entity);
		void GenerateParticlesReversed(const Entity& entity);
	};
}
