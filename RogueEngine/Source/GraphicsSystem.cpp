#include "GraphicsSystem.h"
#include "Timer.h"
#include "Main.h"
#include "REMath.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include "FontSystem.h"

namespace Rogue
{
	GraphicsSystem::GraphicsSystem()
		: System(SystemID::id_GRAPHICSSYSTEM), m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 },
		m_FBO{ 0 }, m_texColourBuffer{ 0 }, m_RBO{ 0 },
		m_screenShader{ }, m_shader{ }, 
		m_projLocation{ 0 }, m_viewLocation{ 0 }, m_transformLocation{ 0 }, m_filterLocation{ 0 },
		m_frameVAO{ 0 }, m_frameVBO{ 0 }
	{}

	// Public member functions 
	void GraphicsSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_GRAPHICSSYSTEM, GraphicsSystem::Receive);

		// Add components to signature
		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<SpriteComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());

		// Set graphics system signature
		g_engine.m_coordinator.SetSystemSignature<GraphicsSystem>(signature);

		// Init OpenGL libraries.
		RE_ASSERT(InitializeOpenGL(), "OpenGL not initialized");

		// Emplace shaders into the map
		g_engine.m_coordinator.GetShaderManager().Init();

		m_shader = g_engine.m_coordinator.loadShader("Object Shader");
		m_screenShader = g_engine.m_coordinator.loadShader("Screen Shader");

		m_projLocation = glGetUniformLocation(m_shader.GetShader(), "projection");
		m_viewLocation = glGetUniformLocation(m_shader.GetShader(), "view");
		m_transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");
		m_filterLocation = glGetUniformLocation(m_shader.GetShader(), "colourFilter");

		GenerateQuadPrimitive(m_VBO, m_VAO, m_EBO);
		GenerateFrameQuad(m_frameVAO, m_frameVBO);

		auto handle = g_engine.GetWindowHandler();

		GenerateFrameBuffer(m_FBO, m_texColourBuffer, m_RBO, GetWindowWidth(handle), GetWindowHeight(handle));

		m_pCamera = g_engine.m_coordinator.GetSystem<CameraSystem>();

		// OpenGL version
		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void GraphicsSystem::Update()
	{
		g_engine.m_coordinator.InitTimeSystem("Graphics System");

		m_drawQueue.clear();

		// For all entities
		for (auto entity : m_entities)
		{
			auto priority = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity).getDrawPriority();

			m_drawQueue.insert(std::make_pair(priority, entity));
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		//auto msg = g_engine.GetWindowMessage();

		// clear the buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateFrameBuffer();
		//if (TranslateMessage(&msg) == WM_SIZE || TranslateMessage(&msg) == WM_DISPLAYCHANGE)
		//{
		//}

		glUseProgram(m_shader.GetShader());
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		// For all entities
		for (auto pair : m_drawQueue)
		{
			auto& entity = pair.second;

			if (g_engine.m_coordinator.ComponentExists<UIComponent>(entity))
				drawUI(entity);
			else
				draw(entity);
		}

		glUseProgram(0);
		glBindVertexArray(0); //Reset
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		g_engine.m_coordinator.GetSystem<FontSystem>()->TrueUpdate();
		g_engine.m_coordinator.GetSystem<DebugDrawSystem>()->TrueUpdate();

		UseFrameBuffer();

		g_engine.m_coordinator.EndTimeSystem("Graphics System");
	}

	void GraphicsSystem::draw(Entity& entity)
	{
		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

		auto transformMat = glm::mat4(1.0f);
		auto viewMat = m_pCamera->GetViewMatrix(1.0f);
		auto texture = sprite.getTexture();

		transformMat = glm::translate(transformMat, { transform.getPosition().x, transform.getPosition().y, 1.0f });

		transformMat = glm::rotate(transformMat, transform.getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		// Flip the player depending on direction
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(entity))
			transformMat = glm::scale(transformMat, glm::vec3(m_playerX, 1.0f, 1.0f));

		transformMat = glm::scale(transformMat, glm::vec3(transform.getScale().x, transform.getScale().y, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texture.m_texture);
		UpdateTextureCoords(sprite.getTexCoordMin(), sprite.getTexCoordMax());

		// Parallax
		if (g_engine.m_coordinator.ComponentExists<BackgroundComponent>(entity))
			viewMat = m_pCamera->GetViewMatrix(g_engine.m_coordinator.GetComponent<BackgroundComponent>(entity).GetParallax());

		// model to world, world to view, view to projection
		glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(g_engine.GetProjMat()));
		glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, glm::value_ptr(viewMat));
		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		// rgb filtering
		glUniform4fv(m_filterLocation, 1, glm::value_ptr(sprite.getFilter()));

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void GraphicsSystem::drawUI(Entity& entity)
	{
		// don't draw inactive UI
		if (!g_engine.m_coordinator.GetComponent<UIComponent>(entity).getIsActive())
			return;

		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

		auto transformMat = glm::mat4(1.0f);
		auto texture = sprite.getTexture();

		transformMat = glm::translate(transformMat, { transform.getPosition().x, transform.getPosition().y, 1.0f });
		transformMat = glm::rotate(transformMat, transform.getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformMat = glm::scale(transformMat, glm::vec3(transform.getScale().x, transform.getScale().y, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texture.m_texture);
		UpdateTextureCoords(sprite.getTexCoordMin(), sprite.getTexCoordMax());

		// model to world, world to view, view to projection
		glUniformMatrix4fv(m_projLocation, 1, GL_FALSE, glm::value_ptr(g_engine.GetProjMat()));
		glUniformMatrix4fv(m_viewLocation, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		glUniform4fv(m_filterLocation, 1, glm::value_ptr(sprite.getFilter()));

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void GraphicsSystem::Receive(Event* ev)
	{
		switch (ev->GetEventType())
		{
		case EventType::EvEntityMove:
		{
			//EntMoveEvent* EvEntMove = dynamic_cast<EntMoveEvent*>(ev);
			//ForceManager::instance().RegisterForce(EvEntMove->GetEntityID(),
				//EvEntMove->GetVecMovement(),
				//g_fixedDeltaTime);

			EntMoveEvent* EvEntMove = dynamic_cast<EntMoveEvent*>(ev);
			m_playerX = EvEntMove->GetVecMovement().x;

			return;
		}
		default:
		{
			return;
		}
		}
	}

	void GraphicsSystem::UseFrameBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDisable(GL_DEPTH_TEST);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(m_screenShader.GetShader());
		glBindVertexArray(m_frameVAO);
		glBindTexture(GL_TEXTURE_2D, m_texColourBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);
		glUseProgram(0);
	}

	void GraphicsSystem::UpdateFrameBuffer()
	{
		auto handle = g_engine.GetWindowHandler();

		glBindTexture(GL_TEXTURE_2D, m_texColourBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, GetWindowWidth(handle), GetWindowHeight(handle), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

		glBindRenderbuffer(GL_RENDERBUFFER, m_RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, GetWindowWidth(handle), GetWindowHeight(handle));
	}

	GLuint& GraphicsSystem::getFBO()
	{
		return m_FBO;
	}

	bool GraphicsSystem::InitializeOpenGL()
	{
		// Init OpenGL
		glEnable(GL_TEXTURE_2D);						   // Texture Mapping
		//glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);						   // Smooth shading
		glDepthFunc(GL_LEQUAL);							   // Depth testing type
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations

		// Enable alpha
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "GLEW broke" << std::endl;
			return false;
		}

		return true;
	}

	void GraphicsSystem::Shutdown()
	{}
}
