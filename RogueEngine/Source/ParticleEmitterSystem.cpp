#include "Precompiled.h"
#include "ParticleEmitterSystem.h"
#include "Main.h"
#include "ParticleComponent.h"


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

			switch (pEmitter.GetParticleType())
			{
			case ParticleType::EXPLOSION:
				GenerateExplosion(entity);
				break;
			case ParticleType::SPRAY:
				GenerateSpray(entity);
				break;
			case ParticleType::TRAIL:
				GenerateTrail(entity);
				break;
			}

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

	void ParticleEmitterSystem::GenerateExplosion(const Entity& entity)
	{
		auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		const float& magnitude = pEmitter.GetMagnitude();

		for (int i = 0; i < 100 * magnitude; ++i)
		{
			std::ostringstream strstream;
			Entity particle = g_engine.m_coordinator.CreateEntity();

			TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(particle);
			Vec2 pos = transform.GetPosition();
			strstream << pos.x << ";"
				<< pos.y << ";"
				<< "20;20;0;100";
			ballTransform.Deserialize(strstream.str());

			strstream.str(std::string());;

			Vec2 velocity;
			velocity.y = RandFloat() * 10 * magnitude;

			// split the directions between left and right
			if (rand() % 2)
				velocity.x = RandFloat() * 10 * magnitude;
			else
				velocity.x = -RandFloat() * 10 * magnitude;

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(particle);
			rigidbody.Deserialize("0;0;0;0;1;1;0");
			rigidbody.addForce(velocity);

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.CreateComponent<BoxCollider2DComponent>(particle);
			boxCollider.Deserialize("0;0;0;0;0");

			SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(particle);
			strstream << pEmitter.GetTexturePath().data() << ";1;1;1;1;1";
			sprite.Deserialize(strstream.str());

			ParticleComponent& particleComp = g_engine.m_coordinator.CreateComponent<ParticleComponent>(particle);
			particleComp.SetLifetime(RandFloat(20.0f));
			
			HierarchyInfo newInfo(particle, "Particle " + std::to_string(i));
			g_engine.m_coordinator.GetActiveObjects().push_back(particle);
			g_engine.m_coordinator.GetHierarchyInfo(particle) = newInfo;
		}
	}

	void ParticleEmitterSystem::GenerateSpray(const Entity& entity)
	{

	}

	void ParticleEmitterSystem::GenerateTrail(const Entity& entity)
	{
		auto& pEmitter = g_engine.m_coordinator.GetComponent<ParticleEmitterComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		auto& ownerRBody = g_engine.m_coordinator.GetComponent<RigidbodyComponent>(entity);

		const float& magnitude = pEmitter.GetMagnitude();

		for (int i = 0; i < 100 * magnitude; ++i)
		{
			std::ostringstream strstream;
			Entity particle = g_engine.m_coordinator.CreateEntity();

			TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(particle);
			Vec2 pos = transform.GetPosition();

			// The RandFloat here is to slightly offset the particle
			strstream << pos.x + RandFloat(5.0f) << ";"
				<< pos.y + RandFloat(5.0f) << ";"
				<< "20;20;0";

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(particle);
			rigidbody.Deserialize("0;0;0;0;1;1;0");
			rigidbody.addForce(ownerRBody.getVelocity()/ RandFloat()); // only a fraction of the velocity is applied

			SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(particle);
			strstream << pEmitter.GetTexturePath().data() << ";1;1;1;1;1";
			sprite.Deserialize(strstream.str());

			ParticleComponent& particleComp = g_engine.m_coordinator.CreateComponent<ParticleComponent>(particle);
			particleComp.SetLifetime(RandFloat(20.0f));

			HierarchyInfo newInfo(particle, "Particle " + std::to_string(i));
			g_engine.m_coordinator.GetActiveObjects().push_back(particle);
			g_engine.m_coordinator.GetHierarchyInfo(particle) = newInfo;
		}
	}
}