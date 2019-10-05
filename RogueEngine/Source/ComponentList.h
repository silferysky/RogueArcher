#pragma once

#include "SpriteComponent.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "CircleCollider2DComponent.h"
#include "BoxCollider2DComponent.h"

enum COMPONENTID
{
	SPRITE = 0,
	RIGIDBODY,
	TRANSFORM,
	CIRCLECOLLIDER2D,
	BOXCOLLIDER2D,
	LASTCOMP
};