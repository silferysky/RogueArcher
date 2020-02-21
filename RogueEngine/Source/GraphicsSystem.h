/* Start Header ************************************************************************/
/*!
\file           GraphicsSystem.h
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
#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "GL/glew.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "CameraSystem.h"

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

		GLuint m_frameVAO;
		GLuint m_frameVBO;

		Shader m_shader;
		Shader m_foregroundShader;
		Shader m_screenShader;

		// Uniform Buffer Object
		GLuint m_uboMatrices;

		GLint m_transformLocation;
		GLint m_filterLocation;

		GLint m_foregroundTransformLocation;
		GLint m_foregroundFilterLocation;

		std::shared_ptr<CameraSystem> m_pCamera;
		float m_playerX = 1.0f;

		std::multimap<int, Entity> m_drawQueue;

		bool InitializeOpenGL();
	public:
		GraphicsSystem();
		~GraphicsSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void draw(Entity& entity);
		void drawForeground(Entity& entity);

		void Receive(Event* ev);

		void UseFrameBuffer();
		void UpdateFrameBuffer();

		GLuint& getFBO();
		GLuint& getUBOMatrices();
		Shader& getShader();
	};
}