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

			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			switch (pEmitter.GetParticleType())
			{
			case ParticleType::EXPLOSION:
				GenerateExplosion(pEmitter, transform);
				break;
			case ParticleType::SPRAY:
				GenerateSpray(pEmitter, transform);
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

	void ParticleEmitterSystem::GenerateExplosion(const ParticleEmitterComponent& pEmitter, const TransformComponent& transform)
	{
		const float& magnitude = pEmitter.GetMagnitude();
		for (int i = 0; i < 100 * magnitude; ++i)
		{
			std::ostringstream strstream;
			Entity particle = g_engine.m_coordinator.CreateEntity();

			TransformComponent& ballTransform = g_engine.m_coordinator.CreateComponent<TransformComponent>(particle);
			Vec2 pos = transform.GetPosition();
			strstream << pos.x << ";"
				<< pos.y << ";"
				<< "20;20;0";

			Vec2 velocity;
			velocity.y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 10 * magnitude;

			// split the directions between left and right
			if (rand() % 2)
				velocity.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 10 * magnitude;
			else
				velocity.x = -static_cast <float> (rand()) / static_cast <float> (RAND_MAX) * 10 * magnitude;

			RigidbodyComponent& rigidbody = g_engine.m_coordinator.CreateComponent<RigidbodyComponent>(particle);
			rigidbody.Deserialize("0;0;0;0;1;1;0");
			rigidbody.addForce(velocity);

			BoxCollider2DComponent& boxCollider = g_engine.m_coordinator.CreateComponent<BoxCollider2DComponent>(particle);
			boxCollider.Deserialize("0;0;0;0;0");

			SpriteComponent& sprite = g_engine.m_coordinator.CreateComponent<SpriteComponent>(particle);
			strstream << pEmitter.GetTexturePath().data() << ";1;1;1;1;1";
			sprite.Deserialize(strstream.str());

			ParticleComponent& particleComp = g_engine.m_coordinator.CreateComponent<ParticleComponent>(particle);
			particleComp.SetLifetime(static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f));
			
			HierarchyInfo newInfo(particle, "Particle " + std::to_string(i));
			g_engine.m_coordinator.GetActiveObjects().push_back(particle);
			g_engine.m_coordinator.GetHierarchyInfo(particle) = newInfo;
		}
	}

	void ParticleEmitterSystem::GenerateSpray(const ParticleEmitterComponent& pEmitter, const TransformComponent& transform)
	{

	}
}