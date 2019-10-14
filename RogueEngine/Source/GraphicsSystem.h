#pragma once
#include "SystemManager.h"
#include "EventListener.h"
#include "GL/glew.h"
#include "GLHelper.hpp"
#include "ComponentList.h"

class Timer;
class SpriteComponent;

class GraphicsSystem : public System, public EventListener
{
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_EBO;

	Shader m_shader;
public:
	GraphicsSystem() = default;
	~GraphicsSystem() = default;

	void init();
	void update() override;

	void draw(SpriteComponent* sprite, TransformComponent* transform);

	void receive(Event* ev);
};