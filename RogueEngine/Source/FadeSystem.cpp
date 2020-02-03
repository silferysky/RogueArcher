#include "Precompiled.h"
#include "FadeSystem.h"
#include "GraphicsEvent.h"
#include "Main.h"

namespace Rogue
{
	FadeSystem::FadeSystem()
		: System(SystemID::id_FADESYSTEM)
	{}

	void FadeSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_FADESYSTEM, FadeSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<SpriteComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<FadeComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<FadeSystem>(signature);
	}

	void FadeSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Fade System");

		// For all entities
		for (auto entity : m_entities)
		{
			auto& fade = g_engine.m_coordinator.GetComponent<FadeComponent>(entity);

			if (!fade.getIsActive())
				continue;

			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);

			glm::vec4 colourFilter = sprite.getFilter();

			if (fade.getIsFadingIn() && colourFilter.a < 255)
			{
				colourFilter.a += 0.01f * fade.getFadeVelocity();
				sprite.setFilter(colourFilter);
			}
			else if (colourFilter.a)
			{
				colourFilter.a -= 0.01f * fade.getFadeVelocity();
				sprite.setFilter(colourFilter);
			}

		}

		g_engine.m_coordinator.EndTimeSystem("Fade System");
	}

	void FadeSystem::Shutdown()
	{}

	void FadeSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EvFade:
		{
			FadeEvent* event = dynamic_cast<FadeEvent*>(ev);
			for (auto& entity : m_entities)
			{
				if (entity == event->GetEntityToFade())
				{
					g_engine.m_coordinator.GetComponent<FadeComponent>(entity).setIsActive(true);
					break;
				}
			}
			break;
		}
		}
	}
}