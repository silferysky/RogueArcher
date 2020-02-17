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

		if (m_gameIsPaused && m_stepOnce) // Game is paused and step button is pressed
		{
			// Step and return
			StepUpdate();
			return;
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

		// If placed before ^, will cause memory leak.
		EventDispatcher::instance().Update();
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
		static int step;

		for (step = 0; step < g_engine.GetStepCount(); ++step)
		{
			// Only run physics if game is paused
			if (!m_gameIsPaused && m_gameIsRunning)
			{
				g_engine.m_coordinator.InitTimeSystem("Physics System");
				m_systems[static_cast<int>(SystemID::id_PHYSICSSYSTEM)].second->Update();
				g_engine.m_coordinator.EndTimeSystem("Physics System");
			}

			//g_engine.m_coordinator.InitTimeSystem("Circle Collision System");
			//m_systems[static_cast<int>(SystemID::id_CIRCLECOLLISIONSYSTEM)].second->Update();
			//g_engine.m_coordinator.EndTimeSystem("Circle Collision System");

			g_engine.m_coordinator.InitTimeSystem("Box Collision System");
			m_systems[static_cast<int>(SystemID::id_BOXCOLLISIONSYSTEM)].second->Update();
			g_engine.m_coordinator.EndTimeSystem("Box Collision System");

			//g_engine.m_coordinator.InitTimeSystem("Collision System");
			//m_systems[static_cast<int>(SystemID::id_COLLISIONSYSTEM)].second->Update();
			//g_engine.m_coordinator.EndTimeSystem("Collision System");
		}

		Timer::instance().GetSystemTimes()["Physics System"] *= step;
		Timer::instance().GetSystemTimes()["Circle Collision System"] *= step;
		Timer::instance().GetSystemTimes()["Box Collision System"] *= step;
		Timer::instance().GetSystemTimes()["Collision System"] *= step;
	}

	void SystemManager::StepUpdate()
	{
		--m_stepCounter;

		if (m_stepCounter <= 0)
			m_stepOnce = false;

		// System updates that are before fixed updates are placed here.
		m_systems[static_cast<int>(SystemID::id_INPUTMANAGER)].second->Update();
		m_systems[static_cast<int>(SystemID::id_LOGICSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PARTICLESYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PARTICLEEMITTERSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_CURSORSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PICKINGSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_PLAYERCONTROLLERSYSTEM)].second->Update();
		m_systems[static_cast<int>(SystemID::id_MENUCONTROLLERSYSTEM)].second->Update();

		FixedUpdate();
		Update();

		// If placed before ^, will cause memory leak.
		EventDispatcher::instance().Update();
	}
}