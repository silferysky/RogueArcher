/* Start Header ************************************************************************/
/*!
\file           ParticleEmitterComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ParticleEmitterComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
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

		int m_particleZ = 0;
		float m_arc = 0.0f;
		float m_angle = 0.0f;
		float m_lifetimeLimit = 0.0f;
		bool m_isFading = false;

		Vec2 m_scale;
		Vec2 m_velocityFactor;
		Vec2 m_positionalOffset;
		Vec2 m_spread;

		bool m_isReversed;

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

		void SetParticleZ(const int particleZ);
		int GetParticleZ() const;

		void SetArc(const float arc);
		float GetArc() const;

		void SetAngle(const float angle);
		float GetAngle() const;

		void SetLifetimeLimit(const float lifetimeLimit);
		float GetLifetimeLimit() const;

		void SetIsFading(const bool isFading);
		bool GetIsFading() const;

		void SetScale(const Vec2 scale);
		Vec2 GetScale() const;

		void SetVelocity(const Vec2 velocity);
		Vec2 GetVelocity() const;

		void SetPositionalOffset(const Vec2 positionalOffset);
		Vec2 GetPositionalOffset() const;

		void SetSpread(const Vec2 spread);
		Vec2 GetSpread() const;

		void SetIsReversed(const bool isReversed);
		bool GetIsReversed() const;

		void SetTexturePath(std::string_view texturePath);
		std::string_view GetTexturePath() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void DisplayOnInspector();
	};
}