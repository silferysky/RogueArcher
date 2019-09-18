#ifndef QUAD_H
#define QUAD_H

#include "Triangle.h"

class Quad
{
	float _vertexpos[8]; // 8 floats for 4 points of (x,y)
	std::string _color; // string for feeding in as source code to the shader but write it like an array
public:
	Quad() = default;
	Quad(float* vertexpos, std::string color);
	~Quad() = default;

	void Draw();
	void Update();
};

#endif