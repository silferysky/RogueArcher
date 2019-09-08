#include "Quad.h"

Quad::Quad(float* vertexpos, std::string color): _color(color)
{
	for (int i = 0; i < 8; ++i)
		_vertexpos[i] = *(vertexpos + i);
}

void Quad::Draw()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), _vertexpos, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	std::string vertexShader =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec4 position;"
		"\n"
		"void main()\n"
		"{\n"
		"  gl_Position = position;\n"
		"}\n";

	std::string fragmentShader =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) out vec4 color;"
		"\n"
		"void main()\n"
		"{\n"
		"  color = vec4"
		+ _color +
		";\n"
		"}\n";

	unsigned int shader = CreateShader(vertexShader, fragmentShader);
	glUseProgram(shader);

	glDrawArrays(GL_QUADS, 0, 4);
}