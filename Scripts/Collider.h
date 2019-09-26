#pragma once
#include "BaseComponent.h"

struct Collider : public BaseComponent
{
	bool _isTrigger;

//public:

	Collider(bool trigger = false)
		: BaseComponent(CMP_COLLIDER) {}

	/*inline void isTrigger(const bool& t) { _isTrigger = t; }
	inline bool isTrigger() const { return _isTrigger; }*/
};