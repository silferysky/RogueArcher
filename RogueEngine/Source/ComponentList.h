#pragma once

#include "BaseComponent.h"
#include "SpriteComponent.h"
#include "RigidbodyComponent.h"
#include "TransformComponent.h"
#include "CircleCollider2DComponent.h"
#include "BoxCollider2DComponent.h"
#include "PlayerControllerComponent.h"
#include "LogicComponent.h"
#include "StatsComponent.h"
#include "AnimationComponent.h"
#include "CameraComponent.h"
#include "AudioEmitterComponent.h"
#include "UIComponent.h"
#include "CursorComponent.h"
#include "ColliderComponent.h"
#include "TextComponent.h"
#include "BackgroundComponent.h"
#include "ParticleComponent.h"
#include "ParticleEmitterComponent.h"

namespace Rogue
{
	enum COMPONENTID
	{
		SPRITE = 0,
		RIGIDBODY,
		TRANSFORM,
		CIRCLECOLLIDER2D,
		BOXCOLLIDER2D,
		PLAYERCONTROLLER,
		LOGIC,
		STATS,
		ANIMATION,
		CAMERA,
		AUDIOEMITTER,
		UI,
		CURSOR,
		COLLIDER,
		TEXT,
		BACKGROUND,
		PARTICLEEMITTER,
		PARTICLE,
		LASTCOMP
	};
}