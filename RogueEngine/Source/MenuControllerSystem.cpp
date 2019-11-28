#include "Precompiled.h"
#include "MenuControllerSystem.h"
#include "Main.h"
#include "KeyEvent.h"
#include "GraphicsEvent.h"


namespace Rogue
{
	MenuControllerSystem::MenuControllerSystem()
		:System(SystemID::id_MENUCONTROLLERSYSTEM), m_menuObjs{std::vector<Entity>()}
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
		switch (ev->GetEventType())
		{
		case EventType::EvEntityPicked:
		{
			//Game must specifically be not in editor mode in order for this to be allowed
			if (!g_engine.m_coordinator.GetEditorIsRunning())
				return;

			EntPickedEvent* entPicked = dynamic_cast<EntPickedEvent*>(ev);

			bool UIPicked = false;
			for (Entity ent : m_entities)
			{
				if (ent == entPicked->GetEntityID())
				{
					UIPicked = true;
					break;
				}
			}

			if (UIPicked)
			{
				HierarchyInfo hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(entPicked->GetEntityID());
				
				//For all cases of "How to Play" (Main Menu and Pause Screens)
				if (hierarchyObj.m_objectName == "HowToPlay")
				{
					InitControlHelpMenu();
				}
				//Main Menu Start
				else if (hierarchyObj.m_objectName == "StartBtn")
				{
					SceneManager& sceneManager = SceneManager::instance();
					sceneManager.LoadLevel("Level 8.json");
				}
				//Exit from Main Menu. Cannot exit from game
				else if (hierarchyObj.m_objectName == "QuitBtn")
				{
					g_engine.SetGameIsRunning(false);
				}
				//Resuming game from paused game state
				else if (hierarchyObj.m_objectName == "BackToGame")
				{
					g_engine.m_coordinator.SetPauseState(false);
				}
				//Exit to Main menu
				else if (hierarchyObj.m_objectName == "BackToMenu")
				{
					SceneManager& sceneManager = SceneManager::instance();
					sceneManager.LoadLevel("Level 10.json");
				}
			}

			return;
		}
		case EventType::EvMouseMoved:
		{
			MouseMoveEvent* mouseMove = dynamic_cast<MouseMoveEvent*>(ev);
			KeyPress keycode = mouseMove->GetKeyCode();

			return;
		}

		case EventType::EvKeyTriggered:
		{
			while (m_menuObjs.size())
			{
				g_engine.m_coordinator.AddToDeleteQueue(m_menuObjs.back());
				m_menuObjs.pop_back();
			}

			//Statement here to make sure all commands only apply if game is not running
			if (g_engine.m_coordinator.GameIsActive())
				return;

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

			if (ev->GetEventCat() & EventCatCombinedInput)
			{
				KeyTriggeredCombinedEvent* keytriggeredcombinedev = dynamic_cast<KeyTriggeredCombinedEvent*>(ev);
				KeyPress keycode = keytriggeredcombinedev->GetKeyCode();
				KeyPressSub keycodeSpecial = keytriggeredcombinedev->GetSubKey();

				//Do stuff
				//RE_INFO(keytriggeredcombinedev->ToString());

				return;
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
	void MenuControllerSystem::InitPauseMenu()
	{
	}
	void MenuControllerSystem::InitControlHelpMenu()
	{
		m_menuObjs.push_back(g_engine.m_coordinator.cloneArchetypes("HowToPlay"));
	}
}