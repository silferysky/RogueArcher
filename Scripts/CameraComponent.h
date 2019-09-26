#pragma once
#include "BaseComponent.h"
#include "Vector2.h"
#include <vector>

struct CameraComponent : public BaseComponent
{
	std::vector<Vec2> _cPosition;

	//public:

	CameraComponent(): BaseComponent(CMP_CAMERA)
	{
		_cPosition = std::vector<Vec2>();
	}

	/*inline void isTrigger(const bool& t) { _isTrigger = t; }
	inline bool isTrigger() const { return _isTrigger; }*/
};