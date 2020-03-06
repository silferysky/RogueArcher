#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class TileComponent : BaseComponent
	{
	public:
		std::string Serialize() { return std::string(); }
		void Deserialize(std::string_view deserializeStr) {}
	};
}