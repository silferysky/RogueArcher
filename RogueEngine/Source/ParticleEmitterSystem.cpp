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
		for (auto entity : m_entities)
		{
			auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);

			if (!pEmitter.GetIsActive())
				continue;

			GenerateParticles(entity);

			if (!pEmitter.GetIsContinuous())
				pEmitter.SetIsActive(false);
		}
	}

	void ParticleEmitterSystem::Receive(Event* ev)
	{
	}

	void ParticleEmitterSystem::Shutdown()
	{
	}

	void ParticleEmitterSystem::GenerateParticles(const Entity& entity)
	{
		unitParticle = g_engine.m_coordinator.cloneArchetypes("Particle", false);
		auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		const float& magnitude = pEmitter.GetMagnitude();
		const Vec2& scale = pEmitter.GetScale();
		const Vec2& velocityFactor = pEmitter.GetVelocity();
		const Vec2& positionalOffset = pEmitter.GetPositionalOffset();
		const Vec2& spread = pEmitter.GetSpread();
		const bool& isFading = pEmitter.GetIsFading();

		SpriteComponent& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(unitParticle);
		sprite.setTexturePath(pEmitter.GetTexturePath().data());

		float timeScale = g_engine.GetTimeScale();

		for (int i = 0; i < magnitude * timeScale; ++i)
		{
			Entity particle = g_engine.m_coordinator.clone(unitParticle, false);

			TransformComponent& particleTransform = g_engine.m_coordinator.GetComponent<TransformComponent>(particle);
			Vec2 pos = transform.GetPosition();

			particleTransform.setPosition(Vec2(pos.x + positionalOffset.x + RandFloat(spread.x), pos.y + positionalOffset.y + RandFloat(spread.y)));
			particleTransform.setScale(scale);
			particleTransform.setZ(transform.GetZ());

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
}