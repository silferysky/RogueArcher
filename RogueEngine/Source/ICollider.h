#pragma once
#include "BaseCollider.h"

namespace Rogue
{
	class ICollider
	{
	public:
		virtual BaseCollider& GetCollider() = 0;
	};
}