#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class UIComponent
		: public BaseComponent
	{
		bool m_isActive = true;
	public:
		UIComponent() = default;
		~UIComponent() = default;

		bool getIsActive() const;
		void setIsActive(const bool& isActive);
		void DisplayOnInspector();
		//Serialize
		std::string_view Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;
	};
}