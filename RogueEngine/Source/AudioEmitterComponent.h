#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class AudioEmitterComponent
		: public BaseComponent
	{
	public:
		AudioEmitterComponent() = default;

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}