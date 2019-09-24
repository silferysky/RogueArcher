#include "Quad.h"
#include "EngineIO.h"

Quad::Quad(float* vertexpos, std::string color): _color(color)
{
	for (int i = 0; i < 8; ++i)
		_vertexpos[i] = *(vertexpos + i);
}

void Quad::CreateShaders()
{
	std::string vertexShader = EngineIO::ReadFile("vertexShader.txt");

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
}

void Quad::Draw()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), _vertexpos, GL_STREAM_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	glDrawArrays(GL_QUADS, 0, 4);
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