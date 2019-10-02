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

				sprite.m_effectMat = glm::rotate_slow(sprite.m_effectMat, (GLfloat)glfwGetTime() * -5.0f, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			RE_INFO("Rotated!");
		}

		if (EvPressKey->GetKeyCode() == KeyPress::KeyS)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::scale_slow(sprite.m_effectMat, glm::vec3(1.1f, 1.1f, 0.0f));
			}
			RE_INFO("Scaled Up!");
		}

		if (EvPressKey->GetKeyCode() == KeyPress::KeyW)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::scale_slow(sprite.m_effectMat, glm::vec3(0.9f, 0.9f, 0.0f));
			}
			RE_INFO("Scaled Down!");
		}

		if (EvPressKey->GetKeyCode() == KeyPress::KeyA)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::translate(sprite.m_effectMat, glm::vec3(-0.1f, 0.0f, 0.0f));
			}
		}

		if (EvPressKey->GetKeyCode() == KeyPress::KeyD)
		{
			for (auto entity : m_entities)
			{
				auto& sprite = gEngine.m_coordinator.GetComponent<SpriteComponent>(entity);

				sprite.m_effectMat = glm::translate(sprite.m_effectMat, glm::vec3(0.1f, 0.0f, 0.0f));
			}
		}

		return;
	}
	default:
		return;
	}
}