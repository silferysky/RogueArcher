#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "ShaderManager.h"

static const float quadVertices[40] =
{
	// positions          // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f, 1.0f,    1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,    0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 1.0f   // top left 
};

static constexpr unsigned int quadIndices[6] =
{
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

static const glm::mat4 projMat = glm::ortho(-16.0f * 0.5f, 16.0f * 0.5f, -9.0f * 0.5f, 9.0f * 0.5f, -10.0f, 10.0f);

/* static void GenerateQuadPrimitive(GLuint& VBO, GLuint& VAO, GLuint& EBO)
{
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //Reset
	glBindVertexArray(0); //Reset
} */

/* static void GenerateLinePrimitive(GLuint& VBO, GLuint& VAO, GLuint& EBO)
{
	m_vertexAttribute = 0;
	m_colorAttribute = 1;

	// Generate
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO);

	glBindVertexArray(&VAO);
	glEnableVertexAttribArray(m_vertexAttribute);
	glEnableVertexAttribArray(m_colorAttribute);

	// Vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(m_vertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_vertices), m_vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_vboIds[1]);
	glVertexAttribPointer(m_colorAttribute, 4, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_colors), m_colors, GL_DYNAMIC_DRAW);

	// Cleanup
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
} */

static void drawLine(const Vector2D& p1, const Vector2D& p2) 
{
	float line_vertex[] =
	{
		p1.x, p1.y, p2.x, p2.y
	};
	glVertexPointer(2, GL_FLOAT, 0, line_vertex);
	glDrawArrays(GL_LINES, 0, 2);
}