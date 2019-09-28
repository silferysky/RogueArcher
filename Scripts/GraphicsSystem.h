#pragma once
#include "../Scripts/Main.h"
#include "../Physics/Transform.h"
#include "../Scripts/REMath.h"
#include "../Scripts/EventListener.h"

#include "Quad.h"

class GraphicsSystem : public System
{
public:
	GraphicsSystem() = default;
	~GraphicsSystem() = default;

	void init();
	void update(float dt);
};