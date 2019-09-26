#pragma once
#include "BaseComponent.h"

struct PhysicsMaterial : public BaseComponent
{
	float _density;
	float _friction;
	float _bounciness;

public:

	PhysicsMaterial(float d = 0.0f, float f = 0.0f, float b = 0.0f)
		: BaseComponent(CMP_PHYMAT), _density{ d }, _friction{ f }, _bounciness{ b }{}

	/*inline void density(const float& d) { _density = d; }
	inline float density() const { return _density; }
	inline void friction(const float& f) { _friction = f; }
	inline float friction() const{ return _friction; }
	inline void bounciness(const float& b) { _bounciness = b; }
	inline float bounciness() const { return _bounciness; }*/
};