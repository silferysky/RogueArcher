#include "Precompiled.h"
#include "LightingSystem.h"
#include "Main.h"
#include "CameraManager.h"

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
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<LightingSystem>(signature);

		m_shader = g_engine.m_coordinator.loadShader("Lighting Shader");

		m_projLocation = glGetUniformLocation(m_shader.GetShader(), "projection");
		m_viewLocation = glGetUniformLocation(m_shader.GetShader(), "view");
		m_transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");

		m_pCamera = g_engine.m_coordinator.GetSystem<CameraSystem>();
		m_graphicsShader = g_engine.m_coordinator.GetSystem<GraphicsSystem>()->getShader();

		m_totalLightsLocation = glGetUniformLocation(m_graphicsShader.GetShader(), "totalLights");

		GenerateQuadPrimitive(m_VBO, m_VAO, m_EBO);
	}

	void LightingSystem::Update()
	{
	}

	void LightingSystem::draw(Entity& entity)
	{
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

		auto transformMat = glm::mat4(1.0f);
		auto viewMat = m_pCamera->GetViewMatrix(1.0f);

		transformMat = glm::translate(transformMat, { transform.GetPosition().x, transform.GetPosition().y, 1.0f });
		transformMat = glm::rotate(transformMat, transform.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformMat = glm::scale(transformMat, glm::vec3(10.0f));

		// model to world, world to view, view to projection
		glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(g_engine.GetProjMat()));
		glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void LightingSystem::TrueUpdate()
	{
		g_engine.m_coordinator.InitTimeSystem("Lighting System");

		glUseProgram(m_shader.GetShader());
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		// For all entities
		for (auto entity : m_entities)
		{
			//++totalLights;
			if (!g_engine.m_coordinator.GetGameState())
				draw(entity);
		}

		glUseProgram(0);
		glBindVertexArray(0); //Reset
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(m_graphicsShader.GetShader());

		glUniform1i(m_totalLightsLocation, totalLights);
		glUniform3fv(glGetUniformLocation(m_graphicsShader.GetShader(), "viewPos"), 1, glm::value_ptr(CameraManager::instance().GetCameraPos()));

		// For all entities
		for (auto entity : m_entities)
		{
			//--totalLights;

			//UpdateShader(entity);
		}

		glUseProgram(0);

		g_engine.m_coordinator.EndTimeSystem("Lighting System");
	}

	void LightingSystem::UpdateShader(Entity& entity)
	{
		auto& light = g_engine.m_coordinator.GetComponent<LightComponent>(entity);
		auto position = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).GetPosition();

		std::string lightLocation = "light[" + std::to_string(totalLights) + ']';

		float ambient = light.getAmbientFactor();
		float diffuse = light.getDiffuseFactor();
		float specular = light.getSpecularFactor();

		glUniform3f(glGetUniformLocation(m_graphicsShader.GetShader(), (lightLocation + ".position").c_str()), position.x, position.y, 1.0f);
		glUniform3f(glGetUniformLocation(m_graphicsShader.GetShader(), (lightLocation + ".specular").c_str()), specular, specular, specular);
		glUniform3f(glGetUniformLocation(m_graphicsShader.GetShader(), (lightLocation + ".ambient").c_str()), ambient, ambient, ambient);
		glUniform3f(glGetUniformLocation(m_graphicsShader.GetShader(), (lightLocation + ".diffuse").c_str()), diffuse, diffuse, diffuse);
		glUniform3f(glGetUniformLocation(m_graphicsShader.GetShader(), (lightLocation + ".specular").c_str()), specular, specular, specular);
	}

	void LightingSystem::Shutdown()
	{}

	void LightingSystem::Receive(Event* ev)
	{}
}