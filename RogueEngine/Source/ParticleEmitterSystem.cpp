/* Start Header ************************************************************************/
/*!
\file           ParticleEmitterSystem.cpp
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
#include "Precompiled.h"
#include "ParticleEmitterSystem.h"
#include "Main.h"
#include "ParticleEmitterComponent.h"

#define PI 3.1415926535897932

namespace Rogue
{
	ParticleEmitterSystem::ParticleEmitterSystem()
		:System(SystemID::id_PARTICLEEMITTERSYSTEM)
	{

	}

	void ParticleEmitterSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_PARTICLEEMITTERSYSTEM, ParticleEmitterSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<ParticleEmitterComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		g_engine.m_coordinator.SetSystemSignature<ParticleEmitterSystem>(signature);

	}

	void ParticleEmitterSystem::Update()
	{
		//g_engine.m_coordinator.InitTimeSystem("Particle Emitter System");

		for (auto entity : m_entities)
		{
			auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);

			if (!pEmitter.GetIsActive())
				continue;

			if (pEmitter.GetIsReversed())
				GenerateParticlesReversed(entity);
			else
				GenerateParticles(entity);

			if (!pEmitter.GetIsContinuous())
				pEmitter.SetIsActive(false);
		}

		//g_engine.m_coordinator.EndTimeSystem("Particle Emitter System");
	}

	void ParticleEmitterSystem::Receive(Event& ev)
	{
	}

	void ParticleEmitterSystem::Shutdown()
	{
	}

	void ParticleEmitterSystem::GenerateParticles(const Entity& entity)
	{
		unitParticle = g_engine.m_coordinator.CloneArchetypes("Particle", false, false);
		auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		Vec2 pos = transform.GetPosition();
		const float& magnitude = pEmitter.GetMagnitude();
		const Vec2& scale = pEmitter.GetScale();
		const Vec2& velocityFactor = pEmitter.GetVelocity();
		const Vec2& positionalOffset = pEmitter.GetPositionalOffset();
		const Vec2& spread = pEmitter.GetSpread();
		const bool& isFading = pEmitter.GetIsFading();
		const int& particleZ = pEmitter.GetParticleZ();

		SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(unitParticle);
		sprite.setTexturePath(pEmitter.GetTexturePath().data());

		float timeScale = g_engine.GetTimeScale();

		for (int i = 0; i < magnitude * timeScale; ++i)
		{
			Entity particle = g_engine.m_coordinator.Clone(unitParticle, false);

			TransformComponent& particleTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(particle);

			particleTransform.setPosition(Vec2(pos.x + positionalOffset.x + RandFloat(spread.x), pos.y + positionalOffset.y + RandFloat(spread.y)));
			particleTransform.setScale(scale);
			particleTransform.setZ(particleZ);

			double arc = pEmitter.GetArc() / 2.0;
			float maxAngle = static_cast<float>((static_cast<double>(pEmitter.GetAngle()) + arc) * PI / 180.0);
			float minAngle = static_cast<float>((static_cast<double>(pEmitter.GetAngle()) - arc) * PI / 180.0);

			float angle = RandFloat(minAngle, maxAngle);
			
			Vec2 velocity;
			velocity.x = RandFloat() * 1000 * velocityFactor.x * cos(angle);
			velocity.y = RandFloat() * 1000 * velocityFactor.y * sin(angle);

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(particle);
			rigidbody.addForce(velocity);

			ParticleComponent& particleComp = g_engine.m_coordinator.CreateComponent<ParticleComponent>(particle);
			particleComp.SetLifetime(RandFloat(pEmitter.GetLifetimeLimit()));
			particleComp.SetIsFading(RandFloat(isFading));
		}

		g_engine.m_coordinator.AddToDeleteQueue(unitParticle);
	}

	void ParticleEmitterSystem::GenerateParticlesReversed(const Entity& entity)
	{
		unitParticle = g_engine.m_coordinator.CloneArchetypes("Particle", false);
		auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		Vec2 pos = transform.GetPosition();
		const float& magnitude = pEmitter.GetMagnitude();
		const Vec2& scale = pEmitter.GetScale();
		const Vec2& velocityFactor = pEmitter.GetVelocity();
		const Vec2& positionalOffset = pEmitter.GetPositionalOffset();
		const Vec2& spread = pEmitter.GetSpread();
		const bool& isFading = pEmitter.GetIsFading();
		const int& particleZ = pEmitter.GetParticleZ();

		SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(unitParticle);
		sprite.setTexturePath(pEmitter.GetTexturePath().data());

		float timeScale = g_engine.GetTimeScale();

		for (int i = 0; i < magnitude * timeScale; ++i)
		{
			Entity particle = g_engine.m_coordinator.Clone(unitParticle, false);
			TransformComponent& particleTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(particle);

			particleTransform.setScale(scale);
			particleTransform.setZ(particleZ);

			double arc = pEmitter.GetArc() / 2.0;
			float maxAngle = static_cast<float>((static_cast<double>(pEmitter.GetAngle()) + arc)* PI / 180.0);
			float minAngle = static_cast<float>((static_cast<double>(pEmitter.GetAngle()) - arc)* PI / 180.0);

			float angle = RandFloat(minAngle, maxAngle);

			Vec2 velocity;
			velocity.x = RandFloat() * 1000 * velocityFactor.x * cos(angle);
			velocity.y = RandFloat() * 1000 * velocityFactor.y * sin(angle);

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(particle);
			rigidbody.addForce(velocity);

			float lifetime = RandFloat(pEmitter.GetLifetimeLimit());

			ParticleComponent& particleComp = g_engine.m_coordinator.CreateComponent<ParticleComponent>(particle);
			particleComp.SetLifetime(lifetime);

			Vec2 position = Vec2(pos.x + velocity.x * lifetime, pos.y + velocity.y * lifetime);

			particleTransform.setPosition(position);
		}

		g_engine.m_coordinator.AddToDeleteQueue(unitParticle);
	}
}