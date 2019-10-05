#pragma once
#ifndef GRAPICSSYSTEM_H
#define GRAPICSSYSTEM_H
#include "Timer.h"
#include "../Source/Main.h"
#include "TransformComponent.h"
#include "../Source/REMath.h"
#include "../Source/EventDispatcher.h"
#include "GLHelper.h"

class Timer;
class GraphicsSystem : public System, public EventListener
{
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	GLuint m_shader;
public:
	GraphicsSystem() = default;
	~GraphicsSystem() = default;

	void init();
	void update() override;

	void receive(Event* ev);
};
#endif