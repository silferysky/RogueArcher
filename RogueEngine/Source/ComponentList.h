/* Start Header ************************************************************************/
/*!
\file           ComponentList.h
\project        Exale
\author         Chan Wai Kit Terence, c.terence, 440005918 (100%)
\par            c.terence\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ComponentList

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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
#include "MaskingComponent.h"
#include "FadeComponent.h"
#include "LightComponent.h"
#include "ForegroundComponent.h"
#include "SoulComponent.h"
#include "ChildComponent.h" // THis is not to be initialized by ObjectFactory
#include "TileMapComponent.h" // Same ^
#include "SaveComponent.h"

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
		MASKING,
		FADE,
		LIGHT,
		FOREGROUND,
		SOUL,
		
		CHILD,
		TILE, 
		SAVE,
		LASTCOMP
	};
}