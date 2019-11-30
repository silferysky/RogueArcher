#include "Precompiled.h"
#include "UISystem.h"
#include "Main.h"
#include "CameraManager.h"

namespace Rogue
{
	UISystem::UISystem()
		: System(SystemID::id_UISYSTEM)
	{}

	void UISystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_UISYSTEM, UISystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<UIComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<UISystem>(signature);

	}

	void UISystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("UI System");

		auto cameraSys = g_engine.m_coordinator.GetSystem<CameraSystem>();

		float lerp = cameraSys->GetLerp();
		m_newCameraPos = CameraManager::instance().GetCameraPos();

		// For all entities
		for (auto entity : m_entities)
		{
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			Vec2 position = transform.GetPosition();

			position.x += static_cast<int>((m_newCameraPos.x - m_oldCameraPos.x));
			position.y += static_cast<int>((m_newCameraPos.y - m_oldCameraPos.y));

			m_oldCameraPos = m_newCameraPos;
		}

		g_engine.m_coordinator.EndTimeSystem("UI System");
	}

	void UISystem::Shutdown()
	{}

	void UISystem::Receive(Event* ev)
	{}
}