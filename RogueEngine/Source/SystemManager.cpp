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

namespace Rogue
{
	// Note: Debug draw system currently doesn't update here.
	void SystemManager::UpdateSystems()
	{
		//std::cout << "Is Transiting: " << m_transitionLevel << std::endl;
		//std::cout << "Transition Time Left " << m_transitionTime << std::endl;
		
		if (m_transitionLevel && m_transitionTime <= 0.0f)
		{
			ImGuiTileSet::instance().ClearTileset();
			SceneManager::instance().LoadLevel(m_transitionString.c_str());

			if (!m_repeatLoad)
				m_transitionLevel = false;
			else
				m_repeatLoad = false;
			
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
		UpdateSystem(SystemID::id_LOGICSYSTEM);
		UpdateSystem(SystemID::id_SOULSYSTEM);
		UpdateSystem(SystemID::id_PLAYERCONTROLLERSYSTEM);
		UpdateSystem(SystemID::id_CURSORSYSTEM);
		UpdateSystem(SystemID::id_PICKINGSYSTEM);
		UpdateSystem(SystemID::id_MENUCONTROLLERSYSTEM);

		FixedUpdate();
		Update();
	}

	void SystemManager::FixedUpdate()
	{
		// Fixed update
		static int step;

		for (step = 0; step < g_engine.GetStepCount(); ++step)
		{
			FixedUpdateSystem(SystemID::id_PARENTCHILDSYSTEM, "Parent Child System");

			// Only run physics if game is running
			if (!m_gameIsPaused && m_gameIsRunning)
			{
				FixedUpdateSystem(SystemID::id_PARTICLESYSTEM, "Particle System");
				FixedUpdateSystem(SystemID::id_PARTICLEEMITTERSYSTEM, "Particle Emitter System");
				FixedUpdateSystem(SystemID::id_FADESYSTEM, "Fade System");
				FixedUpdateSystem(SystemID::id_ANIMATIONSYSTEM, "Animation System");
				FixedUpdateSystem(SystemID::id_UISYSTEM, "UI System");
				FixedUpdateSystem(SystemID::id_PHYSICSSYSTEM, "Physics System");
			}

			//FixedUpdateSystem(SystemID::id_CIRCLECOLLISIONSYSTEM, "Circle Collision System");
			FixedUpdateSystem(SystemID::id_BOXCOLLISIONSYSTEM, "Box Collision System");
			//FixedUpdateSystem(SystemID::id_COLLISIONSYSTEM, "Collision System");
			FixedUpdateSystem(SystemID::id_CAMERASYSTEM, "Camera System");
		}
		
		if (!m_gameIsPaused && m_gameIsRunning)
		{
			Timer::instance().GetSystemTimes()["Physics System"] *= step;
			Timer::instance().GetSystemTimes()["Particle System"] *= step;
			Timer::instance().GetSystemTimes()["Particle Emitter System"] *= step;
			Timer::instance().GetSystemTimes()["Fade System"] *= step;
			Timer::instance().GetSystemTimes()["Animation System"] *= step;
			Timer::instance().GetSystemTimes()["UI System"] *= step;
		}

		//Timer::instance().GetSystemTimes()["Circle Collision System"] *= step;
		Timer::instance().GetSystemTimes()["Box Collision System"] *= step;
		Timer::instance().GetSystemTimes()["Parent Child System"] *= step;
		//Timer::instance().GetSystemTimes()["Collision System"] *= step;
		Timer::instance().GetSystemTimes()["Camera System"] *= step;
	}

	void SystemManager::Update()
	{
		UpdateSystem(SystemID::id_GRAPHICSSYSTEM);
		UpdateSystem(SystemID::id_MASKINGSYSTEM);
		UpdateSystem(SystemID::id_FONTSYSTEM);
		UpdateSystem(SystemID::id_AUDIOSYSTEM);
		UpdateSystem(SystemID::id_LIGHTINGSYSTEM);

		// Only run editor if editor is running.
		if (m_editorIsRunning)
			UpdateSystem(SystemID::id_EDITOR);
	}

	void SystemManager::FixedUpdateSystem(SystemID id, const char* systemName)
	{
		g_engine.m_coordinator.InitTimeSystem(systemName);
		UpdateSystem(id);
		g_engine.m_coordinator.EndTimeSystem(systemName);
	}

	void SystemManager::SetTransitionLevel(std::string_view levelName, float transitionTime)
	{
		m_transitionString = levelName.data();
		m_transitionTime = transitionTime;
	}

	void SystemManager::SetTransition(bool transition)
	{
		m_transitionLevel = transition;
		m_repeatLoad = true;
	}

	bool SystemManager::TransitFinish() const
	{
		return !m_transitionLevel;
	}

}