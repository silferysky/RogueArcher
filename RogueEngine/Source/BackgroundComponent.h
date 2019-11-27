#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class BackgroundComponent
		: public BaseComponent
	{
		float m_parallax = 1.2f;
	public:
		BackgroundComponent() = default;
		~BackgroundComponent() = default;

		float GetParallax() const;
		void SetParallax(const float& parallax);

		//Serialize
		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}