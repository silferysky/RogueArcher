/* Start Header ************************************************************************/
/*!
\file           GraphicsSystem.cpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for GraphicsSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#include "Precompiled.h"
#include "GraphicsSystem.h"
#include "Timer.h"
#include "Main.h"
#include "REMath.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"
#include "GraphicsEvent.h"
#include "FontSystem.h"
#include "LightingSystem.h"
#include "EditorTileSet.h"

namespace Rogue
{
	GraphicsSystem::GraphicsSystem()
		: System(SystemID::id_GRAPHICSSYSTEM), m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 },
		m_FBO{ 0 }, m_texColourBuffer{ 0 }, m_RBO{ 0 },
		m_screenShader{ }, m_shader{ }, m_foregroundShader{ },
		m_transformLocation{ 0 }, m_filterLocation{ 0 },
		m_uvScaleLocation{ 0 }, m_uvOffsetLocation{ 0 },
		m_frameVAO{ 0 }, m_frameVBO{ 0 },
		m_foregroundTransformLocation{ 0 }, m_foregroundViewLocation{ 0 }, m_foregroundProjectionLocation{ 0 },
		m_foregrounduvScaleLocation{ 0 }, m_foregrounduvOffsetLocation{ 0 }, m_foregroundFilterLocation{ 0 },
		m_uboMatrices{ 0 }, m_totalLightsLocation{ 0 }
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
		bool openGLInitSuccess = InitializeOpenGL();
		RE_ASSERT(openGLInitSuccess, "OpenGL not initialized");

		// Emplace shaders into the map
		ShaderManager::instance().Init();

		m_shader = g_engine.m_coordinator.loadShader("Object Shader");
		m_foregroundShader = g_engine.m_coordinator.loadShader("Foreground Shader");
		m_screenShader = g_engine.m_coordinator.loadShader("Screen Shader");

		GLint m_uniformBlockIndex = glGetUniformBlockIndex(m_shader.GetShader(), "Matrices");
		glUniformBlockBinding(m_shader.GetShader(), m_uniformBlockIndex, 0);

		/* m_uniformBlockIndex = glGetUniformBlockIndex(m_foregroundShader.GetShader(), "Matrices");
		glUniformBlockBinding(m_foregroundShader.GetShader(), m_uniformBlockIndex, 0); */

		// create uniform buffer
		glGenBuffers(1, &m_uboMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		// define the range of the buffer
		glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_uboMatrices, 0, 2 * sizeof(glm::mat4));

		m_transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");
		m_filterLocation = glGetUniformLocation(m_shader.GetShader(), "colourFilter");
		m_uvScaleLocation = glGetUniformLocation(m_shader.GetShader(), "uvScale");
		m_uvOffsetLocation = glGetUniformLocation(m_shader.GetShader(), "uvOffset");

		m_totalLightsLocation = glGetUniformLocation(m_shader.GetShader(), "numLights");

		m_foregroundTransformLocation = glGetUniformLocation(m_foregroundShader.GetShader(), "transform");
		m_foregroundViewLocation = glGetUniformLocation(m_foregroundShader.GetShader(), "view");
		m_foregroundProjectionLocation = glGetUniformLocation(m_foregroundShader.GetShader(), "projection");
		m_foregroundFilterLocation = glGetUniformLocation(m_foregroundShader.GetShader(), "colourFilter");
		m_foregrounduvScaleLocation = glGetUniformLocation(m_foregroundShader.GetShader(), "uvScale");
		m_foregrounduvOffsetLocation = glGetUniformLocation(m_foregroundShader.GetShader(), "uvOffset");

		GenerateQuadPrimitive(m_VBO, m_VAO, m_EBO);
		//GenerateQuadPrimitive(m_instancedVBO, m_instancedVAO, m_instancedEBO);
		GenerateFrameQuad(m_frameVAO, m_frameVBO);

		//entityCount = 0;
		//modelMatrices = new glm::mat4[2048];
		//glGenBuffers(1, &m_instanceBuffer);
		//glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
		// glBufferData(GL_ARRAY_BUFFER, 2048 * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		/*glBindVertexArray(m_instancedVAO);
		// also set instance data
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0); */

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
			auto priority = g_engine.m_coordinator.GetComponent<TransformComponent>(entity).GetZ();

			m_drawQueue.insert(std::make_pair(priority, entity));
		}

		glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		// clear the buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateFrameBuffer();

		glUseProgram(m_shader.GetShader());
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(g_engine.GetProjMat()));

		// For all entities
		for (auto pair : m_drawQueue)
		{
			auto& entity = pair.second;

			if (!g_engine.m_coordinator.ComponentExists<UIComponent>(entity))
			{
				if (!g_engine.m_coordinator.ComponentExists<TileMapComponent>(entity))
					draw(entity);
				else
					drawTilemap(entity);
			}
		}

		glUniform1i(m_totalLightsLocation, 0);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glUseProgram(0);
		glBindVertexArray(0); //Reset
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glUseProgram(m_foregroundShader.GetShader());
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

		glUniformMatrix4fv(m_foregroundProjectionLocation, 1, GL_FALSE, glm::value_ptr(g_engine.GetProjMat()));

		//glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(g_engine.GetProjMat()));

		// For all entities
		for (auto pair : m_drawQueue)
		{
			auto& entity = pair.second;
		
			if (g_engine.m_coordinator.ComponentExists<UIComponent>(entity))
				drawInstanced(entity);
		}

		//glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, 2048);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glUseProgram(0);
		glBindVertexArray(0); //Reset
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		g_engine.m_coordinator.GetSystem<LightingSystem>()->TrueUpdate();
		g_engine.m_coordinator.GetSystem<FontSystem>()->TrueUpdate();
		g_engine.m_coordinator.GetSystem<DebugDrawSystem>()->TrueUpdate();

		g_engine.SwapBuffer();

		UseFrameBuffer();

		//entityCount = 0;

		g_engine.m_coordinator.EndTimeSystem("Graphics System");
	}

	void GraphicsSystem::draw(Entity& entity)
	{
		//Check if it is UI and if UI is not active
		if (g_engine.m_coordinator.ComponentExists<UIComponent>(entity) && !g_engine.m_coordinator.GetComponent<UIComponent>(entity).getIsActive())
			return;

		if (!g_engine.m_coordinator.GetComponent<SpriteComponent>(entity).m_componentIsActive)
			return;

		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

		if (!sprite.m_componentIsActive)
			return;

		glm::mat4 transformMat = glm::mat4(1.0f);
		//glm::mat4 viewMat;
		auto texture = sprite.getTexture();

		transformMat = glm::translate(transformMat, { transform.GetPosition().x, transform.GetPosition().y, 1.0f });
		transformMat = glm::rotate(transformMat, transform.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

		// Flip the player depending on direction
		if (g_engine.m_coordinator.ComponentExists<PlayerControllerComponent>(entity))
			transformMat = glm::scale(transformMat, glm::vec3(m_playerX, 1.0f, 1.0f));

		transformMat = glm::scale(transformMat, glm::vec3(transform.GetScale().x, transform.GetScale().y, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texture.m_texture);
		//UpdateTextureCoords(sprite.getTexCoordMinX(), sprite.getTexCoordMaxX());
		//UpdateTextureCoordsY(sprite.getTexCoordMinY(), sprite.getTexCoordMaxY());

		glUniform2f(m_uvScaleLocation, sprite.getTexCoordScale().x, sprite.getTexCoordScale().y);
		glUniform2f(m_uvOffsetLocation, sprite.getTexCoordOffset().x, sprite.getTexCoordOffset().y);

		// Parallax
		/* if (g_engine.m_coordinator.ComponentExists<BackgroundComponent>(entity))
			viewMat = m_pCamera->GetViewMatrix(g_engine.m_coordinator.GetComponent<BackgroundComponent>(entity).GetParallax());
		else
			viewMat = m_pCamera->GetViewMatrix(); */

		// model to world, world to view, view to projection
		// glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(viewMat));

		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));
		
		if (g_engine.m_coordinator.IsTransitFinish())
		{
			if (m_isFading)
			{
				//std::cout << "Current Fade Factor" << m_currentFadeFactor << std::endl;
				auto tempFilter = sprite.getFilter();
				tempFilter.a *= m_currentFadeFactor;
				glUniform4fv(m_filterLocation, 1, glm::value_ptr(tempFilter));

				//Split to make sure it still prints one more frame when currentFadeFactor is < 0
				if (m_isFadingOut && m_currentFadeFactor < 0.0f)
				{
					m_isFadingOut = false;
					if (m_transitingAfterFade)
					{
						g_engine.m_coordinator.SetTransition(true);
						m_transitingAfterFade = false;
						g_engine.m_coordinator.ResumeMenuButtons();
					}
				}
				else if (m_isFadingOut)
				{
					m_currentFadeFactor -= m_fadeFactor * g_deltaTime;
				}
				else
				{
					m_currentFadeFactor += m_fadeFactor * g_deltaTime;
					if (m_currentFadeFactor > 1.0f)
						m_isFading = false;
				}
			}
			else
				// rgb filtering
				glUniform4fv(m_filterLocation, 1, glm::value_ptr(sprite.getFilter()));
		}

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

	void GraphicsSystem::drawInstanced(Entity& entity)
	{
		if (g_engine.m_coordinator.ComponentExists<UIComponent>(entity) && !g_engine.m_coordinator.GetComponent<UIComponent>(entity).getIsActive())
			return;

		if (!g_engine.m_coordinator.GetComponent<SpriteComponent>(entity).m_componentIsActive)
			return;

		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

		glm::mat4 transformMat = glm::mat4(1.0f);
		auto texture = sprite.getTexture();

		transformMat = glm::translate(transformMat, { transform.GetPosition().x, transform.GetPosition().y, 1.0f });
		transformMat = glm::rotate(transformMat, transform.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformMat = glm::scale(transformMat, glm::vec3(transform.GetScale().x, transform.GetScale().y, 1.0f));

		glBindTexture(GL_TEXTURE_2D, texture.m_texture);
		//UpdateTextureCoords(sprite.getTexCoordMinX(), sprite.getTexCoordMaxX());
		//UpdateTextureCoordsY(sprite.getTexCoordMinY(), sprite.getTexCoordMaxY());

		glm::mat4 viewMat = m_pCamera->GetViewMatrix();
		glUniformMatrix4fv(m_foregroundViewLocation, 1, GL_FALSE, glm::value_ptr(viewMat));

		glUniform2f(m_foregrounduvScaleLocation, sprite.getTexCoordScale().x, sprite.getTexCoordScale().y);
		glUniform2f(m_foregrounduvOffsetLocation, sprite.getTexCoordOffset().x, sprite.getTexCoordOffset().y);
		glUniformMatrix4fv(m_foregroundTransformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		if (g_engine.m_coordinator.IsTransitFinish())
		{
			if (m_isFading)
			{
				//std::cout << "Current Fade Factor" << m_currentFadeFactor << std::endl;
				auto tempFilter = sprite.getFilter();
				tempFilter.a *= m_currentFadeFactor;
				glUniform4fv(m_filterLocation, 1, glm::value_ptr(tempFilter));

				//Split to make sure it still prints one more frame when currentFadeFactor is < 0
				if (m_isFadingOut && m_currentFadeFactor < 0.0f)
				{
					m_isFadingOut = false;
					if (m_transitingAfterFade)
					{
						g_engine.m_coordinator.SetTransition(true);
						m_transitingAfterFade = false;
						g_engine.m_coordinator.ResumeMenuButtons();
					}
				}
				else if (m_isFadingOut)
				{
					m_currentFadeFactor -= m_fadeFactor * g_deltaTime;
				}
				else
				{
					m_currentFadeFactor += m_fadeFactor * g_deltaTime;
					if (m_currentFadeFactor > 1.0f)
						m_isFading = false;
				}
			}
			else
				// rgb filtering
				glUniform4fv(m_foregroundFilterLocation, 1, glm::value_ptr(sprite.getFilter()));
		}

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glBindBuffer(GL_ARRAY_BUFFER, m_instanceBuffer);
		//glBufferData(GL_ARRAY_BUFFER, 2048 * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

		// ++entityCount;
	}

	void GraphicsSystem::drawTilemap(Entity& entity)
	{
		if (!g_engine.m_coordinator.GetComponent<SpriteComponent>(entity).m_componentIsActive)
			return;

		auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
		auto texture = sprite.getTexture();
		glBindTexture(GL_TEXTURE_2D, texture.m_texture);

		auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);
		auto& tileComp = g_engine.m_coordinator.GetComponent<TileMapComponent>(entity);
		auto& tileMap = tileComp.GetTileMap();

		glUniform2f(m_uvScaleLocation, tileComp.GetUVScale().x, tileComp.GetUVScale().y);

		for (auto tile : tileMap) // each tile in the tilemap
		{
			glm::mat4 transformMat = glm::mat4(1.0f);
			//glm::mat4 viewMat;

			transformMat = glm::translate(transformMat, { tile.m_tilePos.x, tile.m_tilePos.y, 1.0f });
			transformMat = glm::rotate(transformMat, transform.GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
			transformMat = glm::scale(transformMat, glm::vec3(transform.GetScale().x, transform.GetScale().y, 1.0f));

			//UpdateTextureCoords(tile.m_min.x, tile.m_max.x);
			//UpdateTextureCoordsY(tile.m_min.y, tile.m_max.y);

			glUniform2f(m_uvOffsetLocation, tile.m_min.x, tile.m_min.y);

			// model to world, world to view, view to projection
			// glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_pCamera->GetViewMatrix()));
			glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

			// rgb filtering
			glUniform4fv(m_filterLocation, 1, glm::value_ptr(sprite.getFilter()));

			// Draw the Mesh
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}

	void GraphicsSystem::Receive(Event& ev)
	{
		switch (ev.GetEventType())
		{
		case EventType::EvEntityMove:
		{
			//EntMoveEvent& EvEntMove = dynamic_cast<EntMoveEvent&>(ev);
			//ForceManager::instance().RegisterForce(EvEntMove->GetEntityID(),
				//EvEntMove->GetVecMovement(),
				//g_fixedDeltaTime);

			EntMoveEvent& EvEntMove = dynamic_cast<EntMoveEvent&>(ev);
			m_playerX = EvEntMove.GetVecMovement().x;

			return;
		}
		case EventType::EvFade:
		{
			FadeEvent& fadeEvent = dynamic_cast<FadeEvent&>(ev);

			if (fadeEvent.GetEntityToFade() == MAX_ENTITIES)
			{
				m_isFading = true;
				m_isFadingOut = true;
				m_currentFadeFactor = 1.0f;
				m_transitingAfterFade = fadeEvent.GetTransitAfterFade();
				if (fadeEvent.GetFadeFactor() != 0.0f)
					m_fadeFactor = fadeEvent.GetFadeFactor();
				else
					m_fadeFactor = 1.0f; //Default is instant
			}
			else
			{
				//Individual fade event not implemented
			}
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

	bool GraphicsSystem::FadeAllEntities()
	{
		bool allFaded = true;

		for (auto& entity : m_entities)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);

			auto colourFilter = sprite.getFilter();

			if (colourFilter.a)
				sprite.setFilter(glm::vec4(colourFilter.r, colourFilter.g, colourFilter.b, colourFilter.a - 0.001));

			if (sprite.getFilter().a)
				allFaded = false;
		}

		return allFaded;
	}

	GLuint& GraphicsSystem::getFBO()
	{
		return m_FBO;
	}

	GLuint& GraphicsSystem::getUBOMatrices()
	{
		return m_uboMatrices;
	}

	Shader& GraphicsSystem::getShader()
	{
		return m_shader;
	}

	bool GraphicsSystem::InitializeOpenGL()
	{
		// Init OpenGL
		glEnable(GL_TEXTURE_2D);						   // Texture Mapping
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);						   // Smooth shading
		glDepthFunc(GL_LEQUAL);							   // Depth testing type
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Perspective Calculations
		glEnable(GL_LINE_SMOOTH);

		// Enable alpha
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "GLEW broke" << std::endl;
			return false;
		}
		glGetError();

		return true;
	}

	void GraphicsSystem::Shutdown()
	{}
}
