#include "MenuControllerSystem.h"
#include "Main.h"
#include "KeyEvent.h"
#include "GraphicsEvent.h"

namespace Rogue
{
	MenuControllerSystem::MenuControllerSystem()
		:System(SystemID::id_MENUCONTROLLERSYSTEM)
	{
	}

	void MenuControllerSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_MENUCONTROLLERSYSTEM, MenuControllerSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<UIComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<SpriteComponent>());

		g_engine.m_coordinator.SetSystemSignature<MenuControllerSystem>(signature);
	}

	void MenuControllerSystem::Update()
	{
		
	}
	void MenuControllerSystem::Receive(Event* ev)
	{
		//Statement here to make sure all commands only apply if game is not running
		if (g_engine.m_coordinator.GameIsActive())
			return;

		switch (ev->GetEventType())
		{
		case EventType::EvMouseMoved:
		{
			MouseMoveEvent* mouseMove = dynamic_cast<MouseMoveEvent*>(ev);
			KeyPress keycode = mouseMove->GetKeyCode();

			return;
		}

		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent* keytriggeredevent = dynamic_cast<KeyTriggeredEvent*>(ev);
			KeyPress keycode = keytriggeredevent->GetKeyCode();

			if (keycode == KeyPress::KeyEsc)
				g_engine.SetGameIsRunning(false);

			if (keycode == KeyPress::KeyF5)
				g_engine.m_coordinator.ToggleEditorIsRunning();

			if (keycode == KeyPress::KeyF6)
				g_engine.ToggleVSync();

			if (keycode == KeyPress::Numpad0 && m_entities.size() > 0)
				g_engine.m_coordinator.clone(*m_entities.begin());

			if (keycode == KeyPress::Numpad1)
				g_engine.m_coordinator.cloneArchetypes("Box");

			if (keycode == KeyPress::Numpad2)
				g_engine.m_coordinator.cloneArchetypes("Circle");

			if (keycode == KeyPress::KeyF8)
			{
				CameraShakeEvent* cameraShakeEvent = new CameraShakeEvent(220.0f);
				EventDispatcher::instance().AddEvent(cameraShakeEvent);
			}

			return;
		}
		default:
		{
		}
		}
	}
	void MenuControllerSystem::Shutdown()
	{
	}
}