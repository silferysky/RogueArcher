/* Start Header ************************************************************************/
/*!
\file           ParticleComponent.h
\project        Exale
\author         Javier Foo, javier.foo, 440002318 (100%)
\par            javier.foo\@digipen.edu
\date           1 December,2019
\brief          This file contains the function definitions for ParticleComponent

All content (C) 2019 DigiPen (SINGAPORE) Corporation, all rights
reserved.

Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*/
/* End Header **************************************************************************/
#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class ParticleComponent
		: public BaseComponent
	{
		float m_lifetime = 0.0f;
		bool m_isFading = false;
	public:
		ParticleComponent() = default;
		~ParticleComponent() = default;

		void SetLifetime(const float lifetime);
		float GetLifetime() const;

		void SetIsFading(const bool isFading);
		bool GetIsFading() const;

		std::string Serialize() override;
		void Deserialize(std::string_view toDeserialize) override;

		void Update(const float dt);
	};
}