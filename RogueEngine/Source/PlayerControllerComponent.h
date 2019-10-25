#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class PlayerControllerComponent
		: public BaseComponent
	{
		bool m_isActive;
	public:
		PlayerControllerComponent() = default;
		~PlayerControllerComponent() = default;

		//No need to serialize PlayerControllerComponent as of now
		std::string Serialize();
		void Deserialize(std::string);
	};

}