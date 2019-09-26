#pragma once
#include "BaseComponent.h"
#include <vector>

struct Animation
{
	//Spritemap var
	int _frameStart;
	int _frameNum;
	int _frameCur;
	float _frameTimer;
	float _framePerSec;

	//public:

	Animation(int fs = 0, int fn = 0, int fc = 0, float ft = 0.0f, float fps = 1.0f)
		: _frameStart{ fs }, _frameNum{ fn }, _frameCur{ fc }, _frameTimer{ ft }, _framePerSec{ fps }{}
};

struct AnimationComponent : public BaseComponent
{
	std::vector<const char*> _AnimName;
	std::vector<Animation> _Anim;
	int _curAnim;
	bool _isAnimating;

	AnimationComponent() 
		: BaseComponent(CMP_ANIMATION)
	{ 
		_curAnim = 0; 
		_isAnimating = false; 
	}

	//Should be handled outside component
	/*void AddAnim(const char* name, Animation anim) 
	{ 
		_AnimName.push_back(name); 
		_Anim.push_back(anim);
	}*/
};