#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "GL/glew.h"
#include "GLHelper.hpp"
#include "ComponentList.h"

namespace Rogue
{
	class Timer;
	class SpriteComponent;

	class GraphicsSystem : public System, public EventListener
	{
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		GLuint m_FBO;
		GLuint m_texColourBuffer;
		GLuint m_RBO;

		Shader m_screenShader;
		Shader m_shader;
		GLint m_transformLocation;

		bool InitializeOpenGL();
	public:
		GraphicsSystem();
		~GraphicsSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void draw(SpriteComponent* sprite, TransformComponent* transform);

		void Receive(Event* ev);

		void UseFrameBuffer();

		GLuint& getFBO();
	};
}