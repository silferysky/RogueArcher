#pragma once
#include "BaseComponent.h"

struct EmitterComponent : public BaseComponent
{
	//Texture var
	float _spreadDis;
	float _spreadAng;
	float _offsetDis;
	float _offsetAng;
	float _particleSpd;
	float _particleSize;
	float _particleAlpha;
	float _spawnRate;
	float _spawnLife;
	float _emitterLife;
	bool _isForever;
	bool _isEmitting;
	bool _isAlphaEnable;

	EmitterComponent(float sd, float sa, float od, float oa, float pSp, float pSi, float pA, float sr, 
					 float sl, float el, bool iF, bool iE, bool iAE)
		: BaseComponent(CMP_EMITTER),
		_spreadDis{ sd }, _spreadAng{ sa }, _offsetDis{ od }, _offsetAng{ oa },
		_particleSpd{ pSp }, _particleSize{ pSi }, _particleAlpha{ pA },
		_spawnRate{ sr }, _spawnLife{ sl }, _emitterLife{ el },
		_isForever{ iF }, _isEmitting{ iE }, _isAlphaEnable{ iAE }{}
};