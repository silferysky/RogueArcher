#include "GraphicsSystem.h"
#include "Timer.h"
#include "Main.h"
#include "REMath.h"
#include "EventDispatcher.h"
#include "KeyEvent.h"

namespace Rogue
{
	GraphicsSystem::GraphicsSystem()
		: System(SystemID::id_GRAPHICSSYSTEM) {}

	// Public member functions 
	void GraphicsSystem::init()
	{
		REGISTER_LISTENER(SystemID::id_GRAPHICSSYSTEM, GraphicsSystem::receive);

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

		m_transformLocation = glGetUniformLocation(m_shader.GetShader(), "transform");

		GenerateQuadPrimitive(m_VBO, m_VAO, m_EBO);

		// OpenGL version
		std::cout << glGetString(GL_VERSION) << std::endl;
	}

	void GraphicsSystem::update()
	{
		g_engine.m_coordinator.InitTimeSystem("Graphics System");
		// clear the buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(m_shader.GetShader());

		// For all entities
		for (auto entity : m_entities)
		{
			auto& sprite = g_engine.m_coordinator.GetComponent<SpriteComponent>(entity);
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			//glDisable(GL_DEPTH_TEST);

			//if (!entity)
			draw(&sprite, &transform);
		}

		glUseProgram(0);

		g_engine.m_coordinator.EndTimeSystem("Graphics System");
	}

	void GraphicsSystem::draw(SpriteComponent* sprite, TransformComponent* transform)
	{
		glBindVertexArray(m_VAO);

		auto transformMat = glm::mat4(1.0f);

		transformMat = glm::translate(transformMat, { transform->getPosition().x, transform->getPosition().y, 1.0f });
		transformMat = glm::rotate(transformMat, transform->getRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		transformMat = glm::scale(transformMat, glm::vec3(transform->getScale().x, transform->getScale().y, 1.0f));

		glBindTexture(GL_TEXTURE_2D, sprite->getTexture());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// model to world, world to view, view to projection

		//offset by translation of camera, inverse of rotation

		transformMat = g_engine.GetProjMat() * transformMat;

		glUniformMatrix4fv(m_transformLocation, 1, GL_FALSE, glm::value_ptr(transformMat));

		// Draw the Mesh
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
		glBindVertexArray(0); //Reset
	}

	void GraphicsSystem::receive(Event* ev)
	{
		/*switch (ev->GetEventType())
		{
		case EventType::EvKeyPressed:
		{
			KeyPressEvent* EvPressKey = dynamic_cast<KeyPressEvent*>(ev);
			return;
		}
		default:
			return;
		}*/
	}

	bool GraphicsSystem::InitializeOpenGL()
	{
		// Init OpenGL
		glEnable(GL_TEXTURE_2D);						   // Texture Mapping
		glEnable(GL_DEPTH_TEST);
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
}
