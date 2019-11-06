#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class BackgroundComponent
		: public BaseComponent
	{
		float m_parallax = 2.0f;
	public:
		BackgroundComponent() = default;
		~BackgroundComponent() = default;

		float GetParallax() const;
		void SetParallax(const float& parallax);

		//Serialize
		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}