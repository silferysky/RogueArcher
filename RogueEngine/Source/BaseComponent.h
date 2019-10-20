#pragma once
#include "ISerializable.h"

class BaseComponent
	: public ISerializable
{
public:
	BaseComponent() = default;
	virtual ~BaseComponent() = default;
};
