#include "triangle.h"

Triangle::Triangle(float f1, float f2, float f3, float f4, float f5, float f6, std::string color) : _vertexpos{ f1, f2, f3, f4, f5, f6 }, _color(color)
{
}

Triangle::Triangle(float* vertexpos, std::string color): _color(color)
{
	for (int i = 0; i < 6; ++i)
		_vertexpos[i] = *(vertexpos + i);
}

void Triangle::Draw()
{
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), _vertexpos, GL_STREAM_DRAW);

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

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::Update()
{
	float sin1 = sin(10*(PI/180));
	float cos1 = cos(10*(PI/180));

	for (int i = 0; i < 6; ++i)
	{
		if (i % 2)
			_vertexpos[i] = _vertexpos[i - 1] * sin1 + _vertexpos[i] * cos1;
		else
			_vertexpos[i] = _vertexpos[i] * cos1 - _vertexpos[i + 1] * sin1;
	}
}

