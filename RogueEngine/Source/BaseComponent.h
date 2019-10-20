#pragma once
#include "ISerializable.h"

namespace Rogue
{
	class BaseComponent
		: public ISerializable
	{
	public:
		BaseComponent() = default;
		virtual ~BaseComponent() = default;
	};
}