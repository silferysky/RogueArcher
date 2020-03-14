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

#define MENU_MAX_POSITION 10000.0f

namespace Rogue
{
	MenuControllerSystem::MenuControllerSystem()
		:System(SystemID::id_MENUCONTROLLERSYSTEM), 
		m_menuObjs{ std::vector<Entity>() }, 
		m_confirmQuitEnt{ std::vector<Entity>() },
		m_confirmQuit{ false },
		m_toMainMenu{ false },
		m_showControlMenu{ false },
		m_showingUI{ false }
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
							hierarchyObj.m_objectName = "ControlHelp";
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
				
				//Main Menu Start
				if (hierarchyObj.m_objectName == "StartBtn")
				{
					ClearMenuObjs();
					g_engine.m_coordinator.SetTransitionLevel("Level 10.json", 0.0f);

					FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f);
					ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
					EventDispatcher::instance().AddEvent(ev);

					g_engine.m_coordinator.SetGameState(true);
					PLAYER_STATUS.SetIndicatorStatus(true);
					HandleMenuObjs();
				}
				//CreditsBtn toggles Credits to be on
				//This is standalone
				else if (hierarchyObj.m_objectName == "CreditsBtn")
				{
					for (auto ent : m_entities)
					{
						if (g_engine.m_coordinator.GetHierarchyInfo(ent).m_objectName == "Credits")
						{
							AudioManager::instance().loadSound("Resources/Sounds/button.ogg", 0.3f, false).Play();
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
					AudioManager::instance().loadSound("Resources/Sounds/button.ogg", 0.3f, false).Play();
					if (auto UI = g_engine.m_coordinator.TryGetComponent<UIComponent>(hierarchyObj.m_Entity))
					{
						UI->get().setIsActive(false);
					}
				}
				//For all cases of "How to Play" (Main Menu and Pause Screens)
				else if (hierarchyObj.m_objectName == "HowToPlay" || hierarchyObj.m_objectName == "HowToPlayBtn" || hierarchyObj.m_objectName == "ControlsBtn")
				{
					if (!m_menuObjs.size())
						return;
					AudioManager::instance().loadSound("Resources/Sounds/button.ogg", 0.3f, false).Play();
					m_showControlMenu = true;

					if (auto UI = g_engine.m_coordinator.TryGetComponent<UIComponent>(m_menuObjs.back()))
					{
						UI->get().setIsActive(true);
					}
				}
				else if (hierarchyObj.m_objectName == "ControlHelp")
				{
					if (!m_menuObjs.size())
						return;
					AudioManager::instance().loadSound("Resources/Sounds/button.ogg", 0.3f, false).Play();
					m_showControlMenu = false;

					if (auto UI = g_engine.m_coordinator.TryGetComponent<UIComponent>(m_menuObjs.back()))
					{
						UI->get().setIsActive(false);
					}
				}
				//Exit from Main Menu. Cannot exit from game
				//else if (hierarchyObj.m_objectName == "QuitBtn")
				//{
				//	g_engine.SetGameIsRunning(false);
				//}
				//Resuming game from paused game state
				else if (hierarchyObj.m_objectName == "Resume")
				{
					//ResumeGame();
					PLAYER_STATUS.SetIndicatorStatus(true);
					m_confirmQuit = false;
					g_engine.m_coordinator.SetPauseState(false);
					g_engine.SetTimeScale(1.0f);
					MoveMenuObjs();
					HandleMenuObjs();
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
				//For quit logic
				else if (hierarchyObj.m_objectName == "Quit" || hierarchyObj.m_objectName == "QuitBtn") 
				{
					m_confirmQuit = true;
					HandleMenuObjs();
				}
				else if (hierarchyObj.m_objectName == "YesBtn")
				{
					if (m_confirmQuit && SceneManager::instance().getCurrentFileName() == "Level 20.json")
						g_engine.SetGameIsRunning(false);
					else
					{
						g_engine.m_coordinator.SetTransitionLevel("Level 20.json", 0.0f);
						FadeEvent ev = FadeEvent(MAX_ENTITIES, 0.5f);
						ev.SetSystemReceivers(static_cast<int>(SystemID::id_GRAPHICSSYSTEM));
						EventDispatcher::instance().AddEvent(ev);
						PLAYER_STATUS.SetIndicatorStatus(false);
					}

				}
				else if (hierarchyObj.m_objectName == "NoBtn")
				{
					m_confirmQuit = false;
					m_showControlMenu = false;
					HandleMenuObjs();
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

#if !DEMO_MODE
			if (keycode == KeyPress::KeyF5)
				g_engine.m_coordinator.ToggleEditorIsRunning();
#endif

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
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("crosshair", true, false));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("MainMenu_Bg", true, false));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("PausedTexture", true, false));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("HowToPlayBtn", true, false));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("MainMenu_Btn", true, false)); //This is useless now
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("Resume", true, false));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("QuitBtn", true, false));
		m_menuObjs.push_back(g_engine.m_coordinator.CloneArchetypes("HowToPlay", true, false));

		m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("ExitBtn", true, false));
		m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("YesBtn", true, false));
		m_confirmQuitEnt.push_back(g_engine.m_coordinator.CloneArchetypes("NoBtn", true, false));

		
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
		ResetMenuPositions();
		SetUIMenuObjs(false);
	}

	void MenuControllerSystem::SetUIMenuObjs(bool newActive)
	{
		for (Entity ent : m_menuObjs)
		{
			//Skips cursor
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

			//if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent))
			//{
			//	sprite->get().m_componentIsActive = newActive;
			//}
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
			//if (auto sprite = g_engine.m_coordinator.TryGetComponent<SpriteComponent>(ent))
			//{
			//	sprite->get().m_componentIsActive = newActive;
			//}
		}
	}

	size_t MenuControllerSystem::GetUIMenuObjsSize()
	{
		return m_menuObjs.size() + m_confirmQuitEnt.size();
	}

	void MenuControllerSystem::ResumeGame()
	{
		g_engine.m_coordinator.SetPauseState(false);
		g_engine.SetTimeScale(1.0f);
		m_confirmQuit = false;

		m_showControlMenu = false;
		//MoveMenuObjs();
		HandleMenuObjs();
	}

	void MenuControllerSystem::MoveMenuObjs()
	{
		bool movingToPos = true;// m_showingUI = !m_showingUI;

		auto itr = m_menuObjsTransforms.begin();

		for (Entity ent : m_menuObjs)
		{
			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(ent))
			{
				if (ent == m_menuObjs.front() + 1)
				{
					std::cout << "INITIAL POS: " << trans->get().GetPosition().x << ";" << trans->get().GetPosition().y << std::endl;
				}
				if (movingToPos)
				{
					if (SceneManager::instance().getCurrentFileName() == "Level 16.json")
					{
						trans->get().setPosition(Vec2((*itr).x + CAMERA_MANAGER.GetCameraPos().x, (*itr).y + CAMERA_MANAGER.GetCameraPos().y));
						++itr;
					}
					else
					{
						trans->get().setPosition(Vec2((*itr).x + CAMERA_MANAGER.GetCameraPos().x, (*itr).y + CAMERA_MANAGER.GetCameraPos().y));
						++itr;
					}
				}
				else
				{
					if (SceneManager::instance().getCurrentFileName() == "Level 16.json")
					{
						trans->get().setPosition(Vec2(MENU_MAX_POSITION, MENU_MAX_POSITION));
						++itr;
					}
					else
					{
						trans->get().setPosition(Vec2(MENU_MAX_POSITION, MENU_MAX_POSITION));
						++itr;
					}
				}
				if (ent == m_menuObjs.front() + 1)
				{
					std::cout << "END POS: " << trans->get().GetPosition().x << ";" << trans->get().GetPosition().y << std::endl;
				}
			}
		}

		for (Entity ent : m_confirmQuitEnt)
		{
			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(ent))
			{
				if (movingToPos)
				{
					if (SceneManager::instance().getCurrentFileName() == "Level 16.json")
					{
						trans->get().setPosition(Vec2((*itr).x + CAMERA_MANAGER.GetCameraPos().x, (*itr).y + CAMERA_MANAGER.GetCameraPos().y));
						++itr;
					}
					else
					{
						trans->get().setPosition(Vec2((*itr).x + CAMERA_MANAGER.GetCameraPos().x, (*itr).y + CAMERA_MANAGER.GetCameraPos().y));
						++itr;
					}
				}
				else
				{
					if (SceneManager::instance().getCurrentFileName() == "Level 16.json")
					{
						trans->get().setPosition(Vec2(MENU_MAX_POSITION, MENU_MAX_POSITION));
						++itr;
					}
					else
					{
						trans->get().setPosition(Vec2(MENU_MAX_POSITION, MENU_MAX_POSITION));
						++itr;
					}
				}
			}
		}
	}

	void MenuControllerSystem::HandleMenuObjs()
	{
		//If Game is Running
		if (g_engine.GetGameIsRunning())
		{
			//If it isn't paused, and not in main menu
			if (!g_engine.m_coordinator.GetPauseState() && SceneManager::instance().getCurrentFileName() != "Level 20.json" && SceneManager::instance().getCurrentFileName() != "Level 19.json")
			{
				for (auto& menuObj : m_menuObjs)
				{
					//Ignore crosshair
					if (menuObj == m_menuObjs.front())
						continue;

					if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
					{
						if (menuObj != m_menuObjs.back())
							ui->get().setIsActive(false);
						else //For control help only
						{
							ui->get().setIsActive(m_showControlMenu);
						}
					}
				}
				for (auto& menuObj : m_confirmQuitEnt)
				{
					if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
					{
						ui->get().setIsActive(false);
					}
				}
			}
			else if (!g_engine.m_coordinator.GetPauseState() && (SceneManager::instance().getCurrentFileName() == "Level 20.json" || SceneManager::instance().getCurrentFileName() == "Level 19.json"))
			{
				for (auto& menuObj : m_menuObjs)
				{
					//Ignore crosshair
					if (menuObj == m_menuObjs.front())
						continue;

					if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
					{
						//Background
						if (m_menuObjs.size() > 1 && menuObj == (m_menuObjs.front() + 1))
						{
							ui->get().setIsActive(m_confirmQuit);
						}
						else if (menuObj != m_menuObjs.back())
						{
							ui->get().setIsActive(false);
						}
						else //For control help only
						{
							ui->get().setIsActive(false);
						}
					}
				}
				for (auto& menuObj : m_confirmQuitEnt)
				{
					if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
					{
						ui->get().setIsActive(m_confirmQuit);
					}
				}
			}
			else //If it is paused
			{
				if (!m_confirmQuit) //If not in confirmation to quit screen
				{
					for (auto& menuObj : m_menuObjs)
					{
						//Ignore crosshair
						if (menuObj == m_menuObjs.front())
							continue;

						if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
						{
							if (menuObj != m_menuObjs.back())
								ui->get().setIsActive(true);
							else //For control help only
								ui->get().setIsActive(m_showControlMenu);
						}
					}
					for (auto& menuObj : m_confirmQuitEnt)
					{
						if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
						{
							ui->get().setIsActive(false);
						}
					}
				}
				else
				{
					for (auto& menuObj : m_menuObjs)
					{
						//Ignore crosshair, background
						if (menuObj == m_menuObjs.front() || menuObj == m_menuObjs.front() + 1)
							continue;

						if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
						{
							if (menuObj != m_menuObjs.back())
								ui->get().setIsActive(false);
						}
					}
					for (auto& menuObj : m_confirmQuitEnt)
					{
						if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
						{
							ui->get().setIsActive(true);
						}
					}
				}
			}
		}
		else //If Game not running
		{
			m_confirmQuit = false;
			for (auto& menuObj : m_menuObjs)
			{
				//Ignore crosshair
				if (menuObj == m_menuObjs.front())
					continue;

				if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
				{
					ui->get().setIsActive(false);
				}
			}
			for (auto& menuObj : m_confirmQuitEnt)
			{
				if (auto ui = g_engine.m_coordinator.TryGetComponent<UIComponent>(menuObj))
				{
					ui->get().setIsActive(false);
				}
			}
		}

	}

	void MenuControllerSystem::ResetMenuPositions()
	{
		m_menuObjsTransforms.clear();

		for (auto& menuEnt : m_menuObjs)
		{
			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(menuEnt))
			{
				m_menuObjsTransforms.push_back(trans->get().GetPosition());
			}
		}
		for (auto& menuEnt : m_confirmQuitEnt)
		{
			if (auto trans = g_engine.m_coordinator.TryGetComponent<TransformComponent>(menuEnt))
			{
				m_menuObjsTransforms.push_back(trans->get().GetPosition());
			}
		}
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