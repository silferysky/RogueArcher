#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class ParticleComponent
		: public BaseComponent
	{
		float m_lifetime = 0.0f;
	public:
		ParticleComponent() = default;
		~ParticleComponent() = default;

		void SetLifetime(const float lifetime);
		float GetLifetime() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void Update(const float dt);
	};
}