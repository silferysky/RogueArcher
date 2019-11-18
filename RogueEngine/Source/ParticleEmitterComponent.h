#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class ParticleEmitterComponent
		: public BaseComponent
	{
	public:
		ParticleEmitterComponent() = default;
		~ParticleEmitterComponent() = default;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}