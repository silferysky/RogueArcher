#include "Precompiled.h"
#include "LightingSystem.h"
#include "Main.h"

namespace Rogue
{
	LightingSystem::LightingSystem()
		: System(SystemID::id_LIGHTINGSYSTEM)
	{}

	void LightingSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_LIGHTINGSYSTEM, LightingSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<LightComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<LightingSystem>(signature);

		m_shader = g_engine.m_coordinator.loadShader("Lighting Shader");

		m_projLocation = glGetUniformLocation(m_shader.GetShader(), "projection");
		m_viewLocation = glGetUniformLocation(m_shader.GetShader(), "view");
		m_transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");

		m_pCamera = g_engine.m_coordinator.GetSystem<CameraSystem>();

		GenerateQuadPrimitive(m_VBO, m_VAO, m_EBO);
	}

	void LightingSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Lighting System");

		// For all entities
		for (auto entity : m_entities)
		{
			
		}

		g_engine.m_coordinator.EndTimeSystem("Lighting System");
	}

	void LightingSystem::draw(Entity& entity)
	{
		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

		auto transformMat = glm::mat4(1.0f);
		auto viewMat = m_pCamera->GetViewMatrix(1.0f);
		auto texture = sprite.getTexture();

		transformMat = glm::translate(transformMat, { transform.GetPosition().x, transform.GetPosition().y, 1.0f });

		transformMat = glm::rotate(transformMat, transform.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		transformMat = glm::scale(transformMat, glm::vec3(transform.GetScale().x, transform.GetScale().y, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texture.m_texture);
		UpdateTextureCoords(sprite.getTexCoordMin(), sprite.getTexCoordMax());

		// Parallax
		if (g_engine.m_coordinator.ComponentExists<BackgroundComponent>(entity))
			viewMat = m_pCamera->GetViewMatrix(g_engine.m_coordinator.GetComponent<BackgroundComponent>(entity).GetParallax());

		// model to world, world to view, view to projection
		glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(g_engine.GetProjMat()));
		glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void LightingSystem::Shutdown()
	{}

	void LightingSystem::Receive(Event* ev)
	{}
}