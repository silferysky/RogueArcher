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
		void DisplayOnInspector();
		void SetSlowTime(const float& slowTime);
		float GetSlowTime() const;
		//No need to serialize PlayerControllerComponent as of now
		std::string_view Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};

}