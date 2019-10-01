#include "GraphicsSystem.h"

// Public member functions 
void GraphicsSystem::init()
{
	
	LISTENER_HANDLER hand = std::bind(&GraphicsSystem::receive, this, std::placeholders::_1);
	EventDispatcher::instance().AddListener(SystemID::id_GRAPHICSSYSTEM, hand);

	// Add components to signature
	Signature signature;
	signature.set(gEngine.m_coordinator.GetComponentType<SpriteComponent>());
//	signature.set(gEngine.m_coordinator.GetComponentType<BoxCollider2D>());
	signature.set(gEngine.m_coordinator.GetComponentType<TransformComponent>());

	// Set graphics system signature
	gEngine.m_coordinator.SetSystemSignature<GraphicsSystem>(signature);

}

void GraphicsSystem::update()
{
	Timer TimeSystem;
	TimeSystem.TimerInit("Graphics System");
	// For all entities
	for (auto entity : m_entities)
	{
		auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = gEngine.m_coordinator.GetComponent<TransformComponent>(entity);
		//auto& collider = gEngine.m_coordinator.GetComponent<BoxCollider2D>(entity);

		//glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		sprite.draw(&transform);

	}
	TimeSystem.TimerEnd("Graphics System");
}

void GraphicsSystem::receive(Event* ev)
{
	switch (ev->GetEventType())
	{
	case EventType::EvKeyPressed:
	{
		KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
		if (EvPressKey->GetKeyCode() == KeyPress::KeyR)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_transformMat = glm::rotate(sprite.m_transformMat, (GLfloat)glfwGetTime() * -0.5f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			RE_INFO("Rotated!");
		}

		if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_transformMat = glm::scale_slow(sprite.m_transformMat, glm::vec3(0.99f, 0.99f, 0.99f));
			}
			RE_INFO("Scaled!");
		}

		return;
	}
	default:
		return;
	}
}