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
		GLint m_projectionLocation;
		GLint m_viewLocation;
	public:
		FontSystem();
		~FontSystem() = default;

		void Init() override;
		void Update() override;
		void Shutdown() override;

		void TrueUpdate();

		void RenderText(const std::string_view& text, Vec2 transform, const Vec2& scale, const glm::vec4& color);

		void Receive(Event* ev);
	};
}