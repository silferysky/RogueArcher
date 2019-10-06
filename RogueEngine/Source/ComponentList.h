#pragma once

#include "BaseComponent.h"
#include "SpriteComponent.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "CircleCollider2DComponent.h"
#include "BoxCollider2DComponent.h"
#include "PlayerControllerComponent.h"

enum COMPONENTID
{
	SPRITE = 0,
	RIGIDBODY,
	TRANSFORM,
	CIRCLECOLLIDER2D,
	BOXCOLLIDER2D,
	PLAYERCONTROLLER,
	LASTCOMP
};