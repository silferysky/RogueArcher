#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "shader.h"

#define PI 3.14159265358979

class Triangle
{
	float _vertexpos[6]; // 6 floats for 3 points of (x,y)
	std::string _color; // string for feeding in as source code to the shader but write it like an array
public:
	Triangle() = default;
	Triangle(float f1, float f2, float f3, float f4, float f5, float f6, std::string color);
	Triangle(float* vertexpos, std::string color);
	~Triangle() = default;

	void Draw();
	void Update();
};

#endif