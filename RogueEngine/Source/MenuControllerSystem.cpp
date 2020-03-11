/* Start Header ************************************************************************/
/*!
\file           MenuControllerSystem.cpp
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for MenuControllerSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "MenuControllerSystem.h"
#include "Main.h"
#include "KeyEvent.h"
#include "GraphicsEvent.h"
#include "CameraManager.h"
#include "PlayerStatusManager.h"

namespace Rogue
{
	MenuControllerSystem::MenuControllerSystem()
		:System(SystemID::id_MENUCONTROLLERSYSTEM), 
		m_menuObjs{ std::vector<Entity>() }, 
		m_confirmQuitEnt{ std::vector<Entity>() },
		m_confirmQuit{ false }
	{
	}

	MenuControllerSystem::~MenuControllerSystem()
	{
		m_menuObjs.clear();
		m_confirmQuitEnt.clear();
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
		g_engine.m_coordinator.InitTimeSystem("Menu Controller System");
		g_engine.m_coordinator.EndTimeSystem("Menu Controller System");
	}

	void MenuControllerSystem::Receive(Event& ev)
	{
		switch (ev.GetEventType())
		{
		case EventType::EvEntityHover:
		{
			EntHoverEvent& hover = dynamic_cast<EntHoverEvent&>(ev);

			if (g_engine.m_coordinator.ComponentExists<SpriteComponent>(hover.GetEntityID()))
			{
				//+2 to skip menuparent, crosshair and background
				if (hover.GetEntityID() < m_menuObjs.front() + 3 || hover.GetEntityID() > *(m_menuObjs.end() - 1))
					return;
				auto filter = g_engine.m_coordinator.GetComponent<SpriteComponent>(hover.GetEntityID()).getFilter();
				filter.a = 0.8f;
				g_engine.m_coordinator.GetComponent<SpriteComponent>(hover.GetEntityID()).setFilter(filter);
			}

			return;
		}
		case EventType::EvEntityPicked:
		{
			//Prevents entity buttons from being useful when game is inactive, allows for pause tho
			if (!g_engine.m_coordinator.GetGameState())
				return;

			EntPickedEvent& entPicked = dynamic_cast<EntPickedEvent&>(ev);

			bool UIPicked = false;
			for (Entity ent : m_entities)
			{
				if (ent == entPicked.GetEntityID())
				{
					UIPicked = true;
					break;
				}
			}

			if (UIPicked)
			{
				HierarchyInfo hierarchyObj = HierarchyInfo();
				
				for (size_t it = 0; it < m_menuObjs.size() + m_confirmQuitEnt.size(); ++it)
				{
					if (entPicked.GetEntityID() == m_menuObjs.front() + it || entPicked.GetEntityID() == m_confirmQuitEnt.front() + it - m_menuObjs.size())
					{
						AudioManager::instance().loadSound("Resources/Sounds/button.ogg", 0.3f, false).Play();
						switch (it)
						{
						//case 0: //MenuParent
						//case 8: //ConfirmBtnParent
							//break;
						case 0: //Crosshair
							break;
						case 1: //PausedTexture
							hierarchyObj.m_objectName = "MainMenu_Bg";
							break;
						case 2: //MainMenu_Bg
							hierarchyObj.m_objectName = "PausedTexture";
							break;
						case 3: //HowToPlayBtn
							hierarchyObj.m_objectName = "HowToPlayBtn";
							break;
						case 4: //MainMenu_Btn
							hierarchyObj.m_objectName = "MainMenu_Btn";
							break;
						case 5: //Resume
							hierarchyObj.m_objectName = "Resume";
							break;
						case 6: //QuitBtn
							hierarchyObj.m_objectName = "Quit";
							break;
						case 7: //HowToPlay
							hierarchyObj.m_objectName = "HowToPlay";
							break;
						case 8: //ExitGame
							hierarchyObj.m_objectName = "ExitGame";
							break;
						case 9: //YesBtn
							hierarchyObj.m_objectName = "YesBtn";
							break;
						case 10: //NoBtn
							hierarchyObj.m_objectName = "NoBtn";
							break;
						}
					}
				}

				if (hierarchyObj.m_objectName == "")
				{
					hierarchyObj = g_engine.m_coordinator.GetHierarchyInfo(entPicked.GetEntityID());
				}
				else
				{
					hierarchyObj.m_Entity = entPicked.GetEntityID();
				}
				
				//For all cases of "How to Play" (Main Menu and Pause Screens)
				if (hierarchyObj.m_objectName == "HowToPlay" || hierarchyObj.m_objectName == "HowToPlayBtn" || hierarchyObj.m_objectName == "ControlsBtn")
				{
					ToggleControlHelpMenu();
					AudioManager::instance().loadSound("Resources/Sounds/button.ogg", 0.3f, false).Play();
				}
				//Main Menu Start
				else if (hierarchyObj.m_objectName == "StartBtn")
				{
					ClearMenuObjs();
					//SceneManager& sceneManager = SceneManager::instance();
					//sceneManager.LoadLevel("Level 12.json");
					g_engine.m_coordinator.SetTransitionLevel("Level 10.json", 0.0f);
					//g_engine.m_coordinator.SetTransition(true);

					FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f);
					ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
					EventDispatcher::instance().AddEvent(ev);

					//Now done in Graphics after loading fin
					//ResumeGame();
					g_engine.m_coordinator.SetGameState(true);
					PLAYER_STATUS.SetIndicatorStatus(true);
				}
				//CreditsBtn toggles Credits to be on
				else if (hierarchyObj.m_objectName == "CreditsBtn")
				{
					for (auto ent : m_entities)
					{
						if (g_engine.m_coordinator.GetHierarchyInfo(ent).m_objectName == "Credits")
						{
							if (auto UI = g_engine.m_coordinator.TryGetComponent<UIComponent>(ent))
							{
								UI->get().setIsActive(true);
							}
							break;
						}
					}
				}
				//If click on Credits, just turn it off
				else if (hierarchyObj.m_objectName == "Credits")
				{
					if (auto UI = g_engine.m_coordinator.TryGetComponent<UIComponent>(hierarchyObj.m_Entity))
					{
						UI->get().setIsActive(false);
					}
				}
				//Exit from Main Menu. Cannot exit from game
				else if (hierarchyObj.m_objectName == "QuitBtn")
				{
					g_engine.SetGameIsRunning(false);
				}
				//Resuming game from paused game state
				else if (hierarchyObj.m_objectName == "Resume")
				{
					ResumeGame();
					PLAYER_STATUS.SetIndicatorStatus(true);
				}
				//Exit to Main menu
				else if (hierarchyObj.m_objectName == "MainMenu_Btn")
				{
					g_engine.m_coordinator.SetTransitionLevel("Level 20.json", 0.0f);
					FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f);
					ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
					EventDispatcher::instance().AddEvent(ev);
					//SceneManager& sceneManager = SceneManager::instance();
					//sceneManager.LoadLevel("Level 1.json");

					//Now done in Graphics after loading fin
					//ResumeGame();
					PLAYER_STATUS.SetIndicatorStatus(false);
				}
				else if (hierarchyObj.m_objectName == "ControlHelp")
				{
					ToggleControlHelpMenu();
					/*if (m_menuObjs.size())
					{
						auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
						for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
						{
							if (*iterator == m_menuObjs.back())
							{
								activeObjects.erase(iterator);
								break;
							}
						}
						g_engine.m_coordinator.DestroyEntity(m_menuObjs.back());
						m_menuObjs.pop_back();
					}*/
				}
				//For quit logic
				else if (hierarchyObj.m_objectName == "Quit") 
				{
					if (m_confirmQuit == false)
					{
						m_confirmQuit = true;
						ToggleQuitButtonObj();
					}
				}
				else if (hierarchyObj.m_objectName == "YesBtn")
				{
					if (m_confirmQuit)
						g_engine.SetGameIsRunning(false);

				}
				else if (hierarchyObj.m_objectName == "NoBtn")
				{
					if (m_confirmQuit)
					{
						m_confirmQuit = false;
						ToggleQuitButtonObj();
					}
				}
			}

			return;
		}
		case EventType::EvMouseMoved:
		{
			MouseMoveEvent& mouseMove = dynamic_cast<MouseMoveEvent&>(ev);
			KeyPress keycode = mouseMove.GetKeyCode();

			return;
		}

		case EventType::EvKeyTriggered:
		{
			KeyTriggeredEvent& keytriggeredevent = dynamic_cast<KeyTriggeredEvent&>(ev);
			KeyPress keycode = keytriggeredevent.GetKeyCode();

			if (keycode == KeyPress::KeyF5)
				g_engine.m_coordinator.ToggleEditorIsRunning();

			if (keycode == KeyPress::KeyF6)
				g_engine.ToggleVSync();

			//Statement here to make sure all commands below only apply if game is not running
			if (!g_engine.m_coordinator.GetGameState())
				return;

			if (g_engine.GetIsFocused())
			{
				if (ev.GetEventCat() & EventCatCombinedInput)
				{
					KeyTriggeredCombinedEvent& keytriggeredcombinedev = dynamic_cast<KeyTriggeredCombinedEvent&>(ev);
					KeyPressSub keycodeSpecial = keytriggeredcombinedev.GetSubKey();

					//Do stuff

					RE_INFO(keytriggeredcombinedev.ToString());
				}

				if (keycode == KeyPress::KeyEsc)
				{
					//if (g_engine.m_coordinator.GetEditorIsRunning())
						//g_engine.SetGameIsRunning(false);
					ResumeGame();
				}

				/*if (keycode == KeyPress::Numpad0 && m_entities.size() > 0)
					g_engine.m_coordinator.clone(*m_entities.begin());

				if (keycode == KeyPress::Numpad1)
					g_engine.m_coordinator.CloneArchetypes("Box");

				if (keycode == KeyPress::Numpad2)
					g_engine.m_coordinator.CloneArchetypes("Circle");*/

				if (keycode == KeyPress::KeyF8)
				{
					CameraShakeEvent cameraShakeEvent(220.0f);
					EventDispatcher::instance().AddEvent(cameraShakeEvent);
				}
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
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("crosshair", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("MainMenu_Bg", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("PausedTexture", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("HowToPlayBtn", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("MainMenu_Btn", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("Resume", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("QuitBtn", true));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("HowToPlay", true));

		m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("ExitBtn", true));
		m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("YesBtn", true));
		m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("NoBtn", true));

		////For camera correctness
		//for (auto& menuEnt : m_menuObjs)
		//{
		//	if (auto menuTrans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(menuEnt))
		//	{
		//		menuTrans->get().setPosition(Vec2(menuTrans->get().GetPosition().x - CAMERA_MANAGER.GetCameraPos().x, menuTrans->get().GetPosition().y - CAMERA_MANAGER.GetCameraPos().y));
		//	}
		//}

		//m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("MenuUI", true));
		//for (auto& child : g_engine.m_coordinator.GetHierarchyInfo(m_menuObjs.front()).m_children)
		//{
		//	m_menuObjs.push_back(child);
		//}

		//m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("MenuConfirmUI", true));
		//for (auto& child : g_engine.m_coordinator.GetHierarchyInfo(m_confirmQuitEnt.front()).m_children)
		//{
		//	m_confirmQuitEnt.push_back(child);
		//}

		SetUIMenuObjs(false);
	}

	void MenuControllerSystem::ToggleControlHelpMenu()
	{
		//Toggle How to play only
		if (m_menuObjs.size())
		{
			if (g_engine.m_coordinator.ComponentExists<UIComponent>(m_menuObjs.back()))
			{
				m_confirmQuit = false;
				UIComponent& ui = g_engine.m_coordinator.GetComponent<UIComponent>(m_menuObjs.back());
				ui.setIsActive(!ui.getIsActive());	
				//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(m_menuObjs.back()))
				//{
				//	glm::vec3 cameraPos = CameraManager::instance().GetCameraPos();
				//	TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(m_menuObjs.back());
				//	if (!ui.getIsActive())
				//		transform.setPosition(Vec2(transform.GetPosition().x - cameraPos.x, transform.GetPosition().y - cameraPos.y));
				//	else
				//		transform.setPosition(Vec2(transform.GetPosition().x + cameraPos.x, transform.GetPosition().y + cameraPos.y));
				//}
			}

			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(m_menuObjs.back()))
			{
				m_confirmQuit = false;
				sprite->get().m_componentIsActive = !sprite->get().m_componentIsActive;
			}
		}
	}

	void MenuControllerSystem::ToggleUIMenuObjs()
	{
		bool movingToPos = true;
		if (m_menuObjs.size())
			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(*m_menuObjs.begin()))
			{
				movingToPos = sprite->get().m_componentIsActive;
			}

		for (Entity ent : m_menuObjs)
		{
			//Skip crosshair
			if (ent == m_menuObjs.front())
				continue;

			//Safety check
			auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent);
			if (!sprite)
				continue;

			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(ent))
			{
				if (movingToPos)
				{
					trans->get().setPosition(Vec2(trans->get().GetPosition().x - CAMERA_MANAGER.GetCameraPos().x, trans->get().GetPosition().y - CAMERA_MANAGER.GetCameraPos().y));
				}
				else
				{
					trans->get().setPosition(Vec2(trans->get().GetPosition().x + CAMERA_MANAGER.GetCameraPos().x, trans->get().GetPosition().y + CAMERA_MANAGER.GetCameraPos().y));
				}
			}
		
			//Do not do last item (ControlHelp)
			if (ent == m_menuObjs.back())
				continue;

			sprite->get().m_componentIsActive = !sprite->get().m_componentIsActive;

			if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(ent))
			{
				ui->get().setIsActive(!ui->get().getIsActive());
			}
		}

		for (Entity ent : m_confirmQuitEnt)
		{
			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(ent))
			{
				if (movingToPos)
				{
					trans->get().setPosition(Vec2(trans->get().GetPosition().x - CAMERA_MANAGER.GetCameraPos().x, trans->get().GetPosition().y - CAMERA_MANAGER.GetCameraPos().y));
				}
				else
				{
					trans->get().setPosition(Vec2(trans->get().GetPosition().x + CAMERA_MANAGER.GetCameraPos().x, trans->get().GetPosition().y + CAMERA_MANAGER.GetCameraPos().y));
				}
			}
		}
	}

	void MenuControllerSystem::SetUIMenuObjs(bool newActive)
	{
		for (Entity ent : m_menuObjs)
		{
			//Skip crosshair
			if (ent == m_menuObjs.front())
				continue;

			//Safety check
			if (g_engine.m_coordinator.ComponentExists<UIComponent>(ent))
			{
				UIComponent& ui = g_engine.m_coordinator.GetComponent<UIComponent>(ent);
				ui.setIsActive(newActive);

				//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(ent))
				//{
				//	glm::vec3 cameraPos = CameraManager::instance().GetCameraPos();
				//	TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(ent);
				//	if (ui.getIsActive())
				//		transform.setPosition(Vec2(transform.GetPosition().x - cameraPos.x, transform.GetPosition().y - cameraPos.y));
				//	else
				//		transform.setPosition(Vec2(transform.GetPosition().x + cameraPos.x, transform.GetPosition().y + cameraPos.y));
				//}
			}

			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent))
			{
				sprite->get().m_componentIsActive = newActive;
			}
		}

		for (Entity ent : m_confirmQuitEnt)
		{
			//Safety check
			if (g_engine.m_coordinator.ComponentExists<UIComponent>(ent))
			{
				UIComponent& ui = g_engine.m_coordinator.GetComponent<UIComponent>(ent);
				ui.setIsActive(newActive);

				//if (g_engine.m_coordinator.ComponentExists<TransformComponent>(ent))
				//{
				//	glm::vec3 cameraPos = CameraManager::instance().GetCameraPos();
				//	TransformComponent& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(ent);
				//	if (ui.getIsActive())
				//		transform.setPosition(Vec2(transform.GetPosition().x - cameraPos.x, transform.GetPosition().y - cameraPos.y));
				//	else
				//		transform.setPosition(Vec2(transform.GetPosition().x + cameraPos.x, transform.GetPosition().y + cameraPos.y));
				//}
			}
			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent))
			{
				sprite->get().m_componentIsActive = newActive;
			}
		}
	}

	void MenuControllerSystem::ToggleQuitButtonObj()
	{
		for (Entity ent : m_menuObjs)
		{
			if (ent == m_menuObjs.front() || ent == m_menuObjs.back() || ent == *(m_menuObjs.begin() + 1))
				continue;

			if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(ent))
			{
				ui->get().setIsActive(!ui->get().getIsActive());
			}
		}

		for (Entity ent : m_confirmQuitEnt)
		{
			//Setting Quit button display to true
			if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(ent))
			{
				ui->get().setIsActive(!ui->get().getIsActive());
			}
			if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent))
			{
				sprite->get().m_componentIsActive = !sprite->get().m_componentIsActive;
			}
		}
	}

	size_t MenuControllerSystem::GetUIMenuObjsSize()
	{
		return m_menuObjs.size() + m_confirmQuitEnt.size();
	}

	void MenuControllerSystem::ResumeGame()
	{
		if (m_confirmQuit)
		{
			ToggleQuitButtonObj();
			m_confirmQuit = false;
		}
		ToggleUIMenuObjs();
		//SetUIMenuObjs(false);
		g_engine.m_coordinator.SetPauseState(false);
		g_engine.SetTimeScale(1.0f);
	}

	void MenuControllerSystem::ClearMenuObjs()
	{
		while (m_menuObjs.size())
		{
			auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
			for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
			{
				if (*iterator == m_menuObjs.back())
				{
					activeObjects.erase(iterator);
					break;
				}
			}
			g_engine.m_coordinator.DestroyEntity(m_menuObjs.back());
			m_menuObjs.pop_back();
		}

		while (m_confirmQuitEnt.size())
		{
			auto& activeObjects = g_engine.m_coordinator.GetActiveObjects();
			for (auto iterator = activeObjects.begin(); iterator != activeObjects.end(); ++iterator)
			{
				if (*iterator == m_confirmQuitEnt.back())
				{
					activeObjects.erase(iterator);
					break;
				}
			}
			g_engine.m_coordinator.DestroyEntity(m_confirmQuitEnt.back());
			m_confirmQuitEnt.pop_back();
		}
	}
}