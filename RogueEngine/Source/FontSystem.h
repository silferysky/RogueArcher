/* Start Header ************************************************************************/
/*!
\file           FontSystem.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for FontSystem

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H

#include "SystemManager.h"
#include "EventListener.h"
#include "GL/glew.h"
#include "GLHelper.hpp"
#include "ComponentList.h"
#include "CameraSystem.h"

namespace Rogue
{
	struct Character
	{
		GLuint     TextureID;  // ID handle of the glyph texture
		glm::ivec2 Size;       // Size of glyph
		glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
		GLuint     Advance;    // Offset to advance to next glyph
	};

	class FontSystem : public System, public EventListener
	{
		std::map<GLchar, Character> Characters;

		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		Shader m_shader;
		
		// Uniform Buffer Object
		GLint m_uniformBlockIndex;
		GLuint m_uboMatrices;

		std::shared_ptr<CameraSystem> m_pCamera;
	public:
		FontSystem();
		~FontSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void TrueUpdate();

		void RenderText(const std::string_view& text, Vec2 transform, const Vec2& scale, const glm::vec4& color);

		void Receive(Event& ev);
	};
}