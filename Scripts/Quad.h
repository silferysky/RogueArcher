#ifndef QUAD_H
#define QUAD_H

#include "Triangle.h"

class Quad
{
	float _vertexpos[8]; // 8 floats for 4 points of (x,y)
	unsigned int shader;
	unsigned int VAO, VBO, EBO;

public:
	Quad() = default;
	Quad(float* vertexpos);
	~Quad() = default;

	void Draw();
	void CreateShaders();
	void RotateRight();
};

#endif