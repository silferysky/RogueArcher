/* Start Header ************************************************************************/
/*!
\file           FontSystem.cpp
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
#include "Precompiled.h"
#include "FontSystem.h"
#include "Timer.h"
#include "Main.h"
#include "REMath.h"
#include "EventDispatcher.h"

namespace Rogue
{
	FontSystem::FontSystem() :
		System(SystemID::id_FONTSYSTEM), m_VAO{ 0 }, m_VBO{ 0 }, m_EBO{ 0 }, m_shader{ }
	{}

	void FontSystem::Init()
	{
		REGISTER_LISTENER(SystemID::id_FONTSYSTEM, FontSystem::Receive);

		Signature signature;
		signature.set(g_engine.m_coordinator.GetComponentType<TextComponent>());
		signature.set(g_engine.m_coordinator.GetComponentType<TransformComponent>());
		g_engine.m_coordinator.SetSystemSignature<FontSystem>(signature);

		FT_Library ft;
		FT_Face face;
		FT_Error ftError = FT_Init_FreeType(&ft);
		FT_Error faceError = FT_New_Face(ft, "Fonts/Pokemon Solid.ttf", 0, &face);

		RE_ASSERT(!ftError, "ERROR - Could not init FreeType Library");

		RE_ASSERT(!faceError, "ERROR - Failed to load font");

		FT_Set_Pixel_Sizes(face, 0, 48);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR - Failed to load Glyph" << std::endl;
				continue;
			}

			// Generate texture
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			// Insert character into map
			Character character = 
			{
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<GLuint>(face->glyph->advance.x)
			};
			Characters.insert(std::pair<GLchar, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		m_shader = g_engine.m_coordinator.loadShader("Font Shader");

		glUseProgram(m_shader.GetShader());
		m_uniformBlockIndex = glGetUniformBlockIndex(m_shader.GetShader(), "Matrices");
		glUniformBlockBinding(m_shader.GetShader(), m_uniformBlockIndex, 0);
		m_uboMatrices = g_engine.m_coordinator.GetSystem<Rogue::GraphicsSystem>()->getUBOMatrices();

		m_pCamera = g_engine.m_coordinator.GetSystem<CameraSystem>();

		// configure buffers
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void FontSystem::Update()
	{
	}

	void FontSystem::TrueUpdate()
	{
		//Timer TimeSystem;
		//TimeSystem.TimerInit("Font System");

		glUseProgram(m_shader.GetShader());
		glBindBuffer(GL_UNIFORM_BUFFER, m_uboMatrices);
		//glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(g_engine.GetProjMat()));
		//glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(m_pCamera->GetViewMatrix()));

		glBindVertexArray(m_VAO);
		glActiveTexture(GL_TEXTURE0);

		for (auto& entity : m_entities)
		{
			auto& text = g_engine.m_coordinator.GetComponent<TextComponent>(entity);
			auto& transform = g_engine.m_coordinator.GetComponent<TransformComponent>(entity);

			RenderText(text.GetWords(), transform.GetPosition(), transform.GetScale(), text.GetColour());
		}

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		//TimeSystem.TimerEnd("Font System");
	}


	void FontSystem::RenderText(const std::string_view& text, Vec2 transform, const Vec2& scale, const glm::vec4& color)
	{
		glUniform4f(glGetUniformLocation(m_shader.GetShader(), "textColor"), color.x, color.y, color.z, color.w);

		// Iterate through all characters
		std::string_view::const_iterator c;
		for (c = text.begin(); c != text.end(); c++)
		{
			Character ch = Characters[*c];

			GLfloat xpos = transform.x + ch.Bearing.x * scale.x;
			GLfloat ypos = transform.y - (ch.Size.y - ch.Bearing.y) * scale.y;

			GLfloat w = ch.Size.x * scale.x;
			GLfloat h = ch.Size.y * scale.y;

			// Update VBO for each character
			GLfloat vertices[6][4] = {
				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos,     ypos,       0.0, 1.0 },
				{ xpos + w, ypos,       1.0, 1.0 },

				{ xpos,     ypos + h,   0.0, 0.0 },
				{ xpos + w, ypos,       1.0, 1.0 },
				{ xpos + w, ypos + h,   1.0, 0.0 }
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.TextureID);

			// Update content of VBO memory
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			// Render quad
			glDrawArrays(GL_TRIANGLES, 0, 6);

			// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
			transform.x += (ch.Advance >> 6)* scale.x; // Bitshift by 6 to get value in pixels (2^6 = 64)
		}
	}

	void FontSystem::Receive(Event& ev)
	{

	}

	void FontSystem::Shutdown()
	{}
}