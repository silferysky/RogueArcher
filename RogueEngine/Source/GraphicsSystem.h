#pragma once
#ifndef GRAPICSSYSTEM_H
#define GRAPICSSYSTEM_H
#include "Timer.h"
#include "../Source/Main.h"
#include "TransformComponent.h"
#include "../Source/REMath.h"
#include "../Source/EventDispatcher.h"

class Timer;
class GraphicsSystem : public System, public EventListener
{
	float quadVertices[40] =
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

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;
public:
	GraphicsSystem() = default;
	~GraphicsSystem() = default;

	void init();
	void update() override;

	void drawDebug(BoxCollider2DComponent* box);

	void receive(Event* ev);
};
#endif