#pragma once
#include "BaseComponent.h"
#include "ParticleType.h"

namespace Rogue
{
	class ParticleEmitterComponent
		: public BaseComponent
	{
		bool m_isActive = true;
		bool m_isContinuous = false;
		enum ParticleType m_type = ParticleType::EXPLOSION;
		float m_magnitude = 0.0f;
		std::string m_texturePath;
	public:
		ParticleEmitterComponent() = default;
		~ParticleEmitterComponent() = default;

		void SetIsActive(const bool isActive);
		bool GetIsActive() const;

		void SetIsContinuous(const bool isContinuous);
		bool GetIsContinuous() const;

		void SetParticleType(const ParticleType& type);
		ParticleType GetParticleType() const;

		void SetMagnitude(const float magnitude);
		float GetMagnitude() const;

		void SetTexturePath(std::string_view texturePath);
		std::string_view GetTexturePath() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}