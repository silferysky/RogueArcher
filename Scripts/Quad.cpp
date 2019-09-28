#include "Quad.h"
#include "EngineIO.h"

Quad::Quad(float* vertexpos)
{
	for (int i = 0; i < 32; ++i)
	{
		_vertexpos[i] = vertices[i];
	}

	std::string vertexShader = EngineIO::ReadFile("vertexShader.txt");

	std::string fragmentShader = EngineIO::ReadFile("fragmentShader.txt");

	_shader = CreateShader(vertexShader, fragmentShader);

	/* for (int i = 0; i < 26; ++i)
	{
		if (!i % 8)
		{
			*(_vertexpos + i) = *(vertexpos + i);
			*(_vertexpos + i + 1) = *(vertexpos + i + 1);
		}
	} */

	glGenVertexArrays(1, &_VAO);
	glGenBuffers(1, &_VBO);
	glGenBuffers(1, &_EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(_vertexpos), _vertexpos, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);//Reset
	glBindVertexArray(0);//Reset
}

void Quad::Draw()
{
	glBindVertexArray(_VAO);
	// Use the shader program for drawing
	glUseProgram(_shader);

	// TODO: Bind Texture

	// Gets the location of the transform in the shader program
	//unsigned int transformLoc = glGetUniformLocation(shader_new, "transform");

	// Calculate the object's transformation matrix
	//glm::mat4 transform{ 1.0f };
	//transform = glm::rotate(transform, timer, glm::vec3(0.0f, 0.0f, 1.0f));

	// Send Transform Matrix to the shader program
	//glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &transform[0][0]);

	// Draw the Mesh
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	// Unbind after drawing
	glBindVertexArray(0);
}