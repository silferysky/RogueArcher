#include "Precompiled.h"
#include "ParticleEmitterSystem.h"
#include "Main.h"
#include "ParticleEmitterComponent.h"


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
		auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		const float& magnitude = pEmitter.GetMagnitude();
		const Vec2& scale = pEmitter.GetScale();
		const Vec2& velocityFactor = pEmitter.GetVelocity();
		const Vec2& positionalOffset = pEmitter.GetPositionalOffset();

		for (int i = 0; i < 100 * magnitude; ++i)
		{
			std::ostringstream strstream;
			Entity particle = g_engine.m_coordinator.CreateEntity();

			TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(particle);
			Vec2 pos = transform.GetPosition();

			strstream << pos.x + RandFloat(positionalOffset.x) << ";"
				      << pos.y + RandFloat(positionalOffset.x) << ";"
				      << scale.x << ";"
					  << scale.y << ";"
				      << "0" << ";"
				      << transform.GetZ();
			ballTransform.Deserialize(strstream.str());

			strstream.str(std::string()); // clear the string stream

			Vec2 velocity;
			velocity.x = RandFloat() * 1000 * velocityFactor.x * cos(pEmitter.GetArc());
			velocity.y = RandFloat() * 1000 * velocityFactor.y * sin(pEmitter.GetArc());

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(particle);
			rigidbody.Deserialize("0;0;0;0;1;1;0");
			rigidbody.addForce(velocity);

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.CreateComponent<BoxCollider2DComponent>(particle);
			boxCollider.Deserialize("0;0;0;0;0");

			SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(particle);
			strstream << pEmitter.GetTexturePath().data() << ";1;1;1;1;1";
			sprite.Deserialize(strstream.str());

			ParticleComponent& particleComp = g_engine.m_coordinator.CreateComponent<ParticleComponent>(particle);
			particleComp.SetLifetime(RandFloat(pEmitter.GetLifetimeLimit()));
		}
	}
}