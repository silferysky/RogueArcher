#include "Precompiled.h"
#include "MaskingSystem.h"
#include "Main.h"

namespace Rogue
{
	MaskingSystem::MaskingSystem()
		: System(SystemID::id_MASKINGSYSTEM)
	{}

	void MaskingSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_MASKINGSYSTEM, MaskingSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<SpriteComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<MaskingComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<MaskingSystem>(signature);
	}

	void MaskingSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Masking System");

		// For all entities
		for (auto entity : m_entities)
		{
			auto& masking = g_engine.m_coordinator.GetComponent<MaskingComponent>(entity);

			if (masking.getIsActive() == false)
				continue;

			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);

			UpdateTexture(&masking, &sprite);
		}

		g_engine.m_coordinator.EndTimeSystem("Masking System");
	}

	void MaskingSystem::UpdateTexture(MaskingComponent* masking, SpriteComponent* sprite)
	{
		double timer = (double)masking->getTimer();
		float maskVelocity = masking->getMaskVelocity();
		double frameSize = (double)masking->getFrameSize();

		double min = timer + maskVelocity;
		double max = 1/frameSize + timer + maskVelocity;

		// reset the frame
		if (max > 1.0)
		{
			if (!masking->getIsLooping())
				masking->setIsActive(false);

			masking->setTimer(0.0f);
		}

		sprite->setTexCoordMinX(static_cast<float>(min));
		sprite->setTexCoordMaxX(static_cast<float>(max));

		if (masking->getIsActive())
			masking->setTimer(static_cast<float>(timer));
	}

	void MaskingSystem::ResetTextures()
	{
		for (auto entity : m_entities)
		{
			auto& masking = g_engine.m_coordinator.GetComponent<MaskingComponent>(entity);
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);

			sprite.setTexCoordMinX(0.0f);
			sprite.setTexCoordMaxX(1.0f / masking.getFrameSize());
		}
	}



	void MaskingSystem::Shutdown()
	{}

	void MaskingSystem::Receive(Event& ev)
	{}
}