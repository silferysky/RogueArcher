#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "SystemManager.h"
#include "EventListener.h"
#include "GL/glew.h"
#include "GLHelper.hpp"
#include "ComponentList.h"

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

		FT_Library ft;
		FT_Face face;
	public:
		FontSystem();
		~FontSystem() = default;

		void init();
		void update() override;

		void RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

		void receive(Event* ev);
	};
}