#pragma once
#include "BaseComponent.h"
#include "Vector2.h"

struct TransformComponent : public BaseComponent
{
	Vec2 _position;
	float _rotation;  //Change to Vec3 in a 3D system
	Vec2 _scale;

//public:

	TransformComponent(Vec2 p = Vec2(), float r = 0.0f, Vec2 s = Vec2()) 
		: BaseComponent(CMP_TRANSFORM), _position{ p }, _rotation{ r }, _scale{ s }{}

	/*inline void position(const Vec2& p) { _position = p; }
	inline Vec2 position() const { return _position; }
	inline void rotation(const float& r) { _rotation = r; }
	inline Vec2 rotation() const { return _rotation; }
	inline void scale(const Vec2& s) { _scale = s; }
	inline Vec2 scale() const { return _scale; */
};