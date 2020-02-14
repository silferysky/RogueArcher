#include "Precompiled.h"
#include "SystemManager.h"

// Systems
#include "InputManager.h"	// Should probably rename to input SYSTEM
#include "PhysicsSystem.h"
#include "CollisionTagSystem.h"
#include "CircleCollisionSystem.h"
#include "BoxCollisionSystem.h"
#include "CollisionSystem.h"
#include "GraphicsSystem.h"
#include "LogicSystem.h"
#include "DebugDrawSystem.h"
#include "FontSystem.h"
#include "PlayerControllerSystem.h"
#include "MenuControllerSystem.h"
#include "AnimationSystem.h"
#include "CameraSystem.h"
#include "AudioSystem.h"
#include "CursorSystem.h"
#include "PickingSystem.h"
#include "ParticleSystem.h"
#include "ParticleEmitterSystem.h"
#include "UISystem.h"
#include "ParentChildSystem.h"
#include "MaskingSystem.h"
#include "FadeSystem.h"
#include "Editor.h"
#include "Main.h"

namespace Rogue
{
	// Note: Debug draw system currently doesn't update here.
	void SystemManager::UpdateSystems()
	{
		// Update cursor
		if (m_gameModeChanged)
		{
			if (m_showCursor)
			{
				m_showCursor = false;
				ShowCursor(false);
			}
			else
			{
				m_showCursor = true;
				ShowCursor(true);
			}
			m_gameModeChanged = false;
		}

		// System updates that are before fixed updates are placed here.
		m_systems[static_cast<int>(SystemID::id_INPUTMANAGER)].second->Update();

		//Skip these systems if game is paused or not running
		if (!m_gameIsPaused && m_gameIsRunning)
		{
			m_systems[static_cast<int>(SystemID::id_LOGICSYSTEM)].second->Update();
			m_systems[static_cast<int>(SystemID::id_PARTICLESYSTEM)].second->Update();
			m_systems[static_cast<int>(SystemID::id_PARTICLEEMITTERSYSTEM)].second->Update();
		}

		m_systems[static_cast<int>(SystemID::id_CURSORSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PICKINGSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PLAYERCONTROLLERSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_MENUCONTROLLERSYSTEM)].second->Update();


		FixedUpdate();
		Update();
	}

	void SystemManager::Update()
	{
		m_systems[static_cast<int>(SystemID::id_GRAPHICSSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_MASKINGSYSTEM)].second->Update();

		// Only update these systems if game is not paused and scene is running
		if (!m_gameIsPaused && m_gameIsRunning)
		{
			m_systems[static_cast<int>(SystemID::id_FADESYSTEM)].second->Update();
			m_systems[static_cast<int>(SystemID::id_ANIMATIONSYSTEM)].second->Update();
		}

		m_systems[static_cast<int>(SystemID::id_FONTSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_CAMERASYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_AUDIOSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_UISYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PARENTCHILDSYSTEM)].second->Update();

		// Only run editor if editor is running.
		if (m_editorIsRunning)
			m_systems[static_cast<int>(SystemID::id_EDITOR)].second->Update();
	}

	void SystemManager::FixedUpdate()
	{
		// Fixed update
		for (int step = 0; step < g_engine.GetStepCount(); ++step)
		{
			// If game is paused, freeze physics system unless step once is called.
			if (m_gameIsPaused && m_stepOnce)
			{
				--m_stepCounter;

				if (m_stepCounter == 0)
					m_stepOnce = false;

				m_systems[static_cast<int>(SystemID::id_PHYSICSSYSTEM)].second->Update();
			}
			else
				m_systems[static_cast<int>(SystemID::id_PHYSICSSYSTEM)].second->Update();

			m_systems[static_cast<int>(SystemID::id_CIRCLECOLLISIONSYSTEM)].second->Update();
			m_systems[static_cast<int>(SystemID::id_BOXCOLLISIONSYSTEM)].second->Update();
			m_systems[static_cast<int>(SystemID::id_COLLISIONSYSTEM)].second->Update();
		}
	}
}