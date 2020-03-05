/* Start Header ************************************************************************/
/*!
\file           GLHelper.hpp
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for GLHelper

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "WindowHelper.h"
#include "Vector2D.h"
#include "ShaderManager.h"

#define glCheckError() glCheckError_(__FILE__, __LINE__) 

namespace Rogue
{
	static const float quadVertices[] =
	{
		// positions          // colors					// texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f  // top left 
	};

	static void UpdateTextureCoords(const float& xMin, const float& xMax)
	{
		float min[] =
		{
			xMin
		};

		float max[] =
		{
			xMax
		};

		glBufferSubData(GL_ARRAY_BUFFER, 7 * sizeof(float), sizeof(float), max);
		glBufferSubData(GL_ARRAY_BUFFER, 16 * sizeof(float), sizeof(float), max);
		glBufferSubData(GL_ARRAY_BUFFER, 25 * sizeof(float), sizeof(float), min);
		glBufferSubData(GL_ARRAY_BUFFER, 34 * sizeof(float), sizeof(float), min);
	}

	static void UpdateTextureCoordsY(const float& yMin, const float& yMax)
	{
		float min[] =
		{
			yMin
		};

		float max[] =
		{
			yMax
		};

		glBufferSubData(GL_ARRAY_BUFFER, 8 * sizeof(float), sizeof(float), max);
		glBufferSubData(GL_ARRAY_BUFFER, 17 * sizeof(float), sizeof(float), min);
		glBufferSubData(GL_ARRAY_BUFFER, 26 * sizeof(float), sizeof(float), min);
		glBufferSubData(GL_ARRAY_BUFFER, 35 * sizeof(float), sizeof(float), max);
	}

	static const float frameVertices[] = 
	{
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	static const float lineVertices[] =
	{
		// positions          // colors
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  // bottom right
	};

	static constexpr unsigned int quadIndices[6] =
	{
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	static void GenerateQuadPrimitive(GLuint& VBO, GLuint& VAO, GLuint& EBO)
	{
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_DYNAMIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		// texture coord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const  void*)(7 * sizeof(float)));
		glEnableVertexAttribArray(2);

		//  normal
		//glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (const  void*)(9 * sizeof(float)));
		//glEnableVertexAttribArray(3);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
		glBindVertexArray(0); //Reset
	}

	static void GenerateLinePrimitive(GLuint& VBO, GLuint& VAO)
	{
		// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertices), lineVertices, GL_DYNAMIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (const void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
		glBindVertexArray(0); //Reset
	}

	static void GenerateFrameQuad(GLuint& frameVAO, GLuint& frameVBO)
	{
		glGenVertexArrays(1, &frameVAO);
		glGenBuffers(1, &frameVBO);

		glBindVertexArray(frameVAO);
		glBindBuffer(GL_ARRAY_BUFFER, frameVBO);

		glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices), &frameVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
		glBindVertexArray(0); //Reset
	}

	static void GenerateFrameBuffer(GLuint& FBO, GLuint& texColourBuffer, GLuint& RBO, const int& width, const int& height)
	{
		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		// create color attachment texture
		glGenTextures(1, &texColourBuffer);
		glBindTexture(GL_TEXTURE_2D, texColourBuffer);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColourBuffer, 0);

		// create a renderbuffer object for depth and stencil attachment
		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
		
		// check if framebuffer is complete
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			std::cout << "Error: Framebuffer is not complete." << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	static void drawLine(const Vector2D& p1, const Vector2D& p2)
	{
		float start[] =
		{
			p1.x, p1.y,
		};

		float end[] =
		{
			p2.x, p2.y
		};

		glBufferSubData(GL_ARRAY_BUFFER, 0, 2 * sizeof(float), start);
		glBufferSubData(GL_ARRAY_BUFFER, 7 * sizeof(float), 2 * sizeof(float), end);

		glDrawArrays(GL_LINES, 0, 2);
	}

	static GLenum glCheckError_(const char* file, int line)
	{
		GLenum errorCode;
		while ((errorCode = glGetError()) != GL_NO_ERROR)
		{
			std::string error;
			switch (errorCode)
			{
			case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
			case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
			case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
			case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
			case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
			case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
			case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
			}
			std::cout << error << " | " << file << " (" << line << ")" << std::endl;
		}
		return errorCode;
	}
}