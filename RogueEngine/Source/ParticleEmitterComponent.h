#pragma once
#include "BaseComponent.h"
#include "Vector2D.h"

namespace Rogue
{
	class ParticleEmitterComponent
		: public BaseComponent
	{
		bool m_isActive = true;
		bool m_isContinuous = false;
		float m_magnitude = 0.0f;

		float m_arc = 0.0f;
		float m_lifetimeLimit = 0.0f;
		float m_fade = 0.0f;

		Vec2 m_scale;
		Vec2 m_velocityFactor;
		Vec2 m_positionalOffset;

		std::string m_texturePath;
	public:
		ParticleEmitterComponent() = default;
		~ParticleEmitterComponent() = default;

		void SetIsActive(const bool isActive);
		bool GetIsActive() const;

		void SetIsContinuous(const bool isContinuous);
		bool GetIsContinuous() const;

		void SetMagnitude(const float magnitude);
		float GetMagnitude() const;

		void SetArc(const float arc);
		float GetArc() const;

		void SetLifetimeLimit(const float lifetimeLimit);
		float GetLifetimeLimit() const;

		void SetFade(const float fade);
		float GetFade() const;

		void SetScale(const Vec2 scale);
		Vec2 GetScale() const;

		void SetVelocity(const Vec2 velocity);
		Vec2 GetVelocity() const;

		void SetPositionalOffset(const Vec2 positionalOffset);
		Vec2 GetPositionalOffset() const;

		void SetTexturePath(std::string_view texturePath);
		std::string_view GetTexturePath() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}