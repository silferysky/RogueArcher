#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class SaveComponent :
		public BaseComponent
	{
	public :
		SaveComponent() = default;
		~SaveComponent() = default;

		//From BaseComponent
		//ISerializable
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}