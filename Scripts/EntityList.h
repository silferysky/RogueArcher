#pragma once
#include "BaseEntity.h"

struct EntPlayer : public BaseEntity
{
	ComponentType _initCmpList[5] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE, CMP_ANIMATION };

	EntPlayer(const char* name) 
		: BaseEntity(name, ENT_PLAYER) {}
};

struct EntArrow : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntArrow(const char* name)
		: BaseEntity(name, ENT_ARROW) {}
};

struct EntAI : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntAI(const char* name)
		: BaseEntity(name, ENT_AI) {}
};

struct EntProp : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntProp(const char* name)
		: BaseEntity(name, ENT_PROP) {}
};

struct EntObstacle : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntObstacle(const char* name)
		: BaseEntity(name, ENT_OBSTACLE) {}
};

struct EntProjectile : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntProjectile(const char* name)
		: BaseEntity(name, ENT_PROJECTILE) {}
};

struct EntWall : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntWall(const char* name)
		: BaseEntity(name, ENT_WALL) {}
};

struct EntBackground : public BaseEntity
{
	ComponentType _initCmpList[2] = { CMP_TRANSFORM, CMP_SPRITE };

	EntBackground(const char* name)
		: BaseEntity(name, ENT_BACKGROUND) {}
};

struct EntCamera : public BaseEntity
{
	ComponentType _initCmpList[1] = { CMP_TRANSFORM };

	EntCamera(const char* name)
		: BaseEntity(name, ENT_CAMERA) {}
};

struct EntParticle : public BaseEntity
{
	ComponentType _initCmpList[2] = { CMP_TRANSFORM, CMP_SPRITE };

	EntParticle(const char* name)
		: BaseEntity(name, ENT_PARTICLE) {}
};

struct EntCursor : public BaseEntity
{
	ComponentType _initCmpList[4] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_RIGIDBODY, CMP_SPRITE };

	EntCursor(const char* name)
		: BaseEntity(name, ENT_MCURSOR) {}
};

struct EntButton : public BaseEntity
{
	ComponentType _initCmpList[3] = { CMP_TRANSFORM, CMP_COLLIDER, CMP_SPRITE };

	EntButton(const char* name)
		: BaseEntity(name, ENT_BUTTON) {}
};