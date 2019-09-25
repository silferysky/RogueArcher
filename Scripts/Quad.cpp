#include "Quad.h"
#include "EngineIO.h"

Quad::Quad(float* vertexpos)
{
	for (int i = 0; i < 8; ++i)
		_vertexpos[i] = *(vertexpos + i);
}

void Quad::CreateShaders()
{
	std::string vertexShader = EngineIO::ReadFile("vertexShader.txt");

	std::string fragmentShader = EngineIO::ReadFile("fragmentShader.txt");

	shader = CreateShader(vertexShader, fragmentShader);

	float vertices[] = {
		// positions          // colors           // texture coords
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
	   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
	   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

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
	glBindVertexArray(VAO);
	// Use the shader program for drawing
	glUseProgram(shader);

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

void Quad::RotateRight()
{
	float sin1 = (float)sin(10 * (PI / 180));
	float cos1 = (float)cos(10 * (PI / 180));

	for (int i = 0; i < 8; ++i)
	{
		if (i % 2)
			_vertexpos[i] = _vertexpos[i - 1] * sin1 + _vertexpos[i] * cos1;
		else
			_vertexpos[i] = _vertexpos[i] * cos1 - _vertexpos[i + 1] * sin1;
	}
}