#pragma once
#include "BaseComponent.h"
#include "Vector2.h"

struct ParticleComponent : public BaseComponent
{
	Vec2 _pos;
	float _lifeTime;
	float _alpha;
	bool _enableAlpha;
	bool _isAlive;

	ParticleComponent(Vec2 p = Vec2(), float lt = 0.0f, float a = 1.0f, bool ea = false, bool ia = false)
		: BaseComponent(CMP_PARTICLE), _pos{ p }, _lifeTime{ lt }, _alpha{ a }, _enableAlpha{ ea }, _isAlive{ ia }{}
};