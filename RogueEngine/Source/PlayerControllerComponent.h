#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class PlayerControllerComponent
		: public BaseComponent
	{
		float m_slowTime;
		bool m_isActive;
	public:
		PlayerControllerComponent() = default;
		~PlayerControllerComponent() = default;
		void setSlowTime(const float& slowTime);
		float getslowTime() const;
		//No need to serialize PlayerControllerComponent as of now
		std::string Serialize();
		void Deserialize(std::string);
	};

}