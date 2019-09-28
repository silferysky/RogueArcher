#ifndef QUAD_H
#define QUAD_H

#include "Triangle.h"

static float vertices[] =
{
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
};

static constexpr unsigned int quadIndices[6] =
{
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

struct Vertex
{
	/* vec3 pos;
	vec4 color;
	vec2 uv; */
};

class Quad
{
	float _vertexpos[32]; // 8 floats for 4 points of (x,y)
	unsigned int _shader;
	unsigned int _VAO, _VBO, _EBO;
public:
	Quad() = default;
	Quad(float* vertexpos);
	~Quad() = default;

	void Draw();
};

#endif