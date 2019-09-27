#pragma once

#include "SpriteComponent.h"
#include "../Physics/Rigidbody.h"
#include "../Physics/Transform.h"
#include "../Physics/CircleCollider2D.h"
#include "../Physics/PhysicsSystem.h"

enum COMPONENTID
{
	SPRITE = 0,
	TRANSFORM,
	RIGIDBODY,
	BOXCOLLIDER2D,
	CIRCLECOLLIDER2D,
	LASTCOMP
};