#pragma once
#ifndef DEBUGDRAWSYSTEM_H
#define DEBUGDRAWSYSTEM_H
#include "Timer.h"
#include "../Source/Main.h"
#include "TransformComponent.h"
#include "../Source/REMath.h"
#include "../Source/EventDispatcher.h"
#include "GLHelper.hpp"

class Timer;
class DebugDrawSystem : public System, public EventListener
{
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	Shader m_shader;
public:
	DebugDrawSystem() = default;
	~DebugDrawSystem() = default;

	void init();
	void update() override;

	void drawAABB(BoxCollider2DComponent* box, TransformComponent* transform);
	void drawOBB(BoxCollider2DComponent* box);
	void drawVelocity(RigidbodyComponent* rBody, TransformComponent* transform);

	void receive(Event* ev);
};
#endif#
