#pragma once

#include "BaseComponent.h"

struct SpriteComponent : public BaseComponent
{
	unsigned int shader;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
};