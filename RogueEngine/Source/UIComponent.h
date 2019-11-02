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

		//Serialize
		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}