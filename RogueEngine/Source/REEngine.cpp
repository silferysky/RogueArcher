#include "Logger.h"
#include "InputManager.h"
#include "EventDispatcher.h"
#include "REEngine.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "Main.h"
#include "Editor.h"
#include "PhysicsSystem.h"
#include "GraphicsSystem.h"
#include "LogicSystem.h"
#include "DebugDrawSystem.h"
#include "FontSystem.h"
#include "CollisionSystem.h"
#include "WindowSystem.h"

namespace Rogue
{
	REEngine::REEngine() :
		m_coordinator{}, m_accumulatedTime{ 0.0f }, m_stepCount{ 0 },
		m_gameIsRunning{ true }
	{}

	void REEngine::RegisterSystems()
	{
		m_coordinator.RegisterSystem<InputManager>();
		m_coordinator.RegisterSystem<LogicSystem>();
		m_coordinator.RegisterSystem<PhysicsSystem>();
		m_coordinator.RegisterSystem<CollisionSystem>();
		m_coordinator.RegisterSystem<WindowSystem>();
		m_coordinator.RegisterSystem<GraphicsSystem>();
		m_coordinator.RegisterSystem<Editor>();
		m_coordinator.RegisterSystem<DebugDrawSystem>();
		m_coordinator.RegisterSystem<FontSystem>();
	}

	void REEngine::RegisterComponents()
	{
		m_coordinator.RegisterComponent<SpriteComponent>();
		m_coordinator.RegisterComponent<RigidbodyComponent>();
		m_coordinator.RegisterComponent<TransformComponent>();
		m_coordinator.RegisterComponent<CircleCollider2DComponent>();
		m_coordinator.RegisterComponent<BoxCollider2DComponent>();
		m_coordinator.RegisterComponent<PlayerControllerComponent>();
		m_coordinator.RegisterComponent<LogicComponent>();
	}

	void REEngine::init()
	{
		// Register all systems.
		RegisterSystems();

		// Register all components
		RegisterComponents();

		// Init systems and system signatures will be set in their respective inits.
		// Other systems and managers will also be initialized here.
		m_coordinator.Init();
	}

	void REEngine::update()
	{
		m_stepCount = 0;
		std::chrono::high_resolution_clock mainLoopTimer;
		gFixedDeltaTime = 0.016f;

		while (m_gameIsRunning)
		{
			//std::cout << 1/gDeltaTime << std::endl;
			gDeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(m_loopEnd - m_loopStart).count() / MICRO_TO_SECONDS;

			m_loopStart = mainLoopTimer.now();

			m_stepCount = 0;

			m_accumulatedTime += gDeltaTime;

			while (m_accumulatedTime >= gFixedDeltaTime)
			{
				m_accumulatedTime -= gFixedDeltaTime;
				m_stepCount++;
			}

			m_coordinator.Update();

			m_loopEnd = mainLoopTimer.now();
		}
	}

	void REEngine::shutdown()
	{
		//put graphics shutdown here
	}

	float REEngine::GetAccumulatedTime() const
	{
		return m_accumulatedTime;
	}

	int REEngine::GetStepCount() const
	{
		return m_stepCount;
	}

	void REEngine::SetGameIsRunning(bool set)
	{
		m_gameIsRunning = set;
	}

}