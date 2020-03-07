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

		GLuint m_instancedVAO;
		GLuint m_instancedVBO;
		GLuint m_instancedEBO;
		GLint m_foregroundTransformLocation;
		GLint m_foregroundFilterLocation;

		std::shared_ptr<CameraSystem> m_pCamera;
		float m_playerX = 1.0f;

		//instancing
		int entityCount;
		glm::mat4* modelMatrices;
		GLuint m_instanceBuffer;

		std::multimap<int, Entity> m_drawQueue;

		bool InitializeOpenGL();
		
		//For Fade In/Out
		float m_fadeFactor = 0.001f;		//Value to fade out by
		float m_currentFadeFactor = 1.0f;	//Current value of fadedness
		bool m_isFading = false;			//Whether the Fade in/Fade out will be called
		bool m_isFadingOut = true;			//Whether it is fading in or out

	public:
		GraphicsSystem();
		~GraphicsSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void draw(Entity& entity);
		void drawInstanced(Entity& entity);

		void Receive(Event& ev);

		void UseFrameBuffer();
		void UpdateFrameBuffer();

		bool FadeAllEntities();

		GLuint& getFBO();
		GLuint& getUBOMatrices();
		Shader& getShader();
	};
}