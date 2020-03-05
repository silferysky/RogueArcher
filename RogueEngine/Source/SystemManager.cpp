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
#include "LightingSystem.h"
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
		//std::cout << "Is Transiting: " << m_transitionLevel << std::endl;
		//std::cout << "Transition Time Left " << m_transitionTime << std::endl;
		if (m_transitionLevel && TransitFinish())
		{
			ImGuiTileSet::instance().ClearTileset();
			SceneManager::instance().LoadLevel(m_transitionString.c_str());
			m_transitionLevel = false;
			
			return;
		}
		else if (m_transitionLevel)
		{
			m_transitionTime -= g_deltaTime; //* g_engine.GetTimeScale();
			return;
		}

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
			while (m_stepCounter > 0)
			{
				for(int steps = 0; steps < g_engine.GetStepCount(); steps++)
					UpdateSystem(SystemID::id_PHYSICSSYSTEM);

				m_stepCounter--;
			}
			m_stepOnce = false;

			return;
		}

		// System updates that are before fixed updates are placed here.
		UpdateSystem(SystemID::id_INPUTMANAGER);

		// Fire events for input
		EventDispatcher::instance().Update();

		//Skip these systems if game is paused or not running
		if (!m_gameIsPaused && m_gameIsRunning)
		{
			UpdateSystem(SystemID::id_LOGICSYSTEM);
			UpdateSystem(SystemID::id_PARTICLESYSTEM);
			UpdateSystem(SystemID::id_PARTICLEEMITTERSYSTEM);

			// Fire events for logic/AI/scripts
			EventDispatcher::instance().Update();
		}

		UpdateSystem(SystemID::id_CURSORSYSTEM);
		UpdateSystem(SystemID::id_PICKINGSYSTEM);
		UpdateSystem(SystemID::id_PLAYERCONTROLLERSYSTEM);
		UpdateSystem(SystemID::id_MENUCONTROLLERSYSTEM);

		FixedUpdate();
		Update();

		// If placed before ^, will cause memory leak.
		EventDispatcher::instance().Update();
	}

	void SystemManager::Update()
	{
		UpdateSystem(SystemID::id_GRAPHICSSYSTEM);
		UpdateSystem(SystemID::id_MASKINGSYSTEM);

		// Only update these systems if game is not paused and scene is running
		if (!m_gameIsPaused && m_gameIsRunning)
		{
			UpdateSystem(SystemID::id_FADESYSTEM);
			UpdateSystem(SystemID::id_ANIMATIONSYSTEM);
		}

		UpdateSystem(SystemID::id_FONTSYSTEM);
		UpdateSystem(SystemID::id_CAMERASYSTEM);
		UpdateSystem(SystemID::id_AUDIOSYSTEM);
		UpdateSystem(SystemID::id_UISYSTEM);
		UpdateSystem(SystemID::id_LIGHTINGSYSTEM);
		UpdateSystem(SystemID::id_PARENTCHILDSYSTEM);

		// Only run editor if editor is running.
		if (m_editorIsRunning)
			UpdateSystem(SystemID::id_EDITOR);
	}

	void SystemManager::SetTransitionLevel(std::string_view levelName, float transitionTime)
	{
		m_transitionLevel = true;
		m_transitionString = levelName.data();
		m_transitionTime = transitionTime;
	}

	bool SystemManager::TransitFinish() const
	{
		return m_transitionTime < 0.0f;
	}

	void SystemManager::FixedUpdate()
	{
		// Fixed update
		static int step;

		for (step = 0; step < g_engine.GetStepCount(); ++step)
		{
			// Only run physics if game is running
			if (!m_gameIsPaused && m_gameIsRunning)
			{
				g_engine.m_coordinator.InitTimeSystem("Physics System");
				UpdateSystem(SystemID::id_PHYSICSSYSTEM);
				g_engine.m_coordinator.EndTimeSystem("Physics System");
			}
			
			//g_engine.m_coordinator.InitTimeSystem("Circle Collision System");
			//UpdateSystem(SystemID::id_CIRCLECOLLISIONSYSTEM);
			//g_engine.m_coordinator.EndTimeSystem("Circle Collision System");
			
			g_engine.m_coordinator.InitTimeSystem("Box Collision System");
			UpdateSystem(SystemID::id_BOXCOLLISIONSYSTEM);
			g_engine.m_coordinator.EndTimeSystem("Box Collision System");
			
			//g_engine.m_coordinator.InitTimeSystem("Collision System");
			//UpdateSystem(SystemID::id_COLLISIONSYSTEM);
			//g_engine.m_coordinator.EndTimeSystem("Collision System");
		}
		
		if (!m_gameIsPaused && m_gameIsRunning)
			Timer::instance().GetSystemTimes()["Physics System"] *= step;
		
		//Timer::instance().GetSystemTimes()["Circle Collision System"] *= step;
		Timer::instance().GetSystemTimes()["Box Collision System"] *= step;
		//Timer::instance().GetSystemTimes()["Collision System"] *= step;

		// Fire events for collisions
		EventDispatcher::instance().Update();
	}
}