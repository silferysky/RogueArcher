Coordinator::Coordinator() :
	m_entityManager{ std::make_unique<EntityManager>() },
	m_componentManager{ std::make_unique<ComponentManager>() },
	m_systemManager{ std::make_unique<SystemManager>() },
	m_textureManager{ std::make_unique<TextureManager>() },
	m_shaderManager{ std::make_unique<ShaderManager>() },
	m_togglePerformanceChecker{ false },
	m_fpscheck{ false }
{}

Coordinator::Init()
{
	m_shaderManager->Init();
	// Init the systems and set their signatures.
	m_systemManager->InitSystems();
}

Coordinator::Update()
{
	// Update the core systems
	m_systemManager->UpdateSystems();
	EventDispatcher::instance().update();
}

inline Entity Coordinator::CreateEntity()
{
	return m_entityManager->CreateEntity();
}


#include "SystemManager.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "GraphicsSystem.h"
#include "DebugDrawSystem.h"
#include "PhysicsSystem.h"
#include "EventDispatcher.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "Coordinator.h"
#include ""