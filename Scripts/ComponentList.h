#pragma once

#include "SpriteComponent.h"
#include "../Physics/RigidbodyComponent.h"
#include "../Physics/TransformComponent.h"
#include "../Physics/CircleCollider2DComponent.h"
#include "../Physics/BoxCollider2DComponent.h"

enum COMPONENTID
{
	SPRITE = 0,
	TRANSFORM,
	RIGIDBODY,
	CIRCLECOLLIDER2D,
	BOXCOLLIDER2D,
	LASTCOMP
};