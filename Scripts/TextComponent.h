#pragma once
#include "BaseComponent.h"
#include "Vector2.h"

struct TextComponent : public BaseComponent
{
	const char* _text;
	Vec2 _position;

	//public:

	TextComponent(const char* t = nullptr, Vec2 pos = Vec2())
		: _text{ t }, _position{ pos }	{}
};