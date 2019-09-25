#pragma once

// Include all components
#include "SpriteComponent.h"
#include "../Physics/Rigidbody.h"
#include "../Physics/Transform.h"
#include "../Physics/Material.h"
#include "../Physics/CircleCollider2D.h"
//#include "../Physics/BoxCollider2D.h"

enum COMPONENTID
{
	SPRITE = 0,
	TRANSFORM,
	RIGIDBODY,
	BOXCOLLIDER2D,
	CIRCLECOLLIDER2D,
	LASTCOMP
};

class BaseComponent
{
public:
	BaseComponent();
	BaseComponent(COMPONENTID id);
	~BaseComponent();

private:
	COMPONENTID ID;
};

