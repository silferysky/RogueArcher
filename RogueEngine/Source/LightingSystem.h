/* Start Header ************************************************************************/
/*!
\file           LightingSystem.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 Decembe, 2019
\brief          This file contains the function declarations for LightingSystem

All content (C) 2020 DigiPen (SINGAPORE) Corporation, all rights
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

	class LightingSystem : public System, public EventListener
	{
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		Shader m_shader;
		Shader m_graphicsShader;

		// Uniform Buffer Object
		GLuint m_uboMatrices;

		GLint m_transformLocation;
		GLint m_totalLightsLocation;

		std::shared_ptr<CameraSystem> m_pCamera;

		unsigned totalLights = 0;

		void draw(Entity& entity);
		void UpdateShader(Entity& entity);
	public:
		LightingSystem();
		~LightingSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void TrueUpdate();

		void Receive(Event* ev);
	};
}
