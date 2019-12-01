/* Start Header ************************************************************************/
/*!
\file           ParticleComponent.cpp
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
#include "Precompiled.h"
#include "ParticleComponent.h"
#include "Main.h"

namespace Rogue
{
	void ParticleComponent::SetLifetime(const float lifetime)
	{
		m_lifetime = lifetime;
	}

	float ParticleComponent::GetLifetime() const
	{
		return m_lifetime;
	}

	void ParticleComponent::SetIsFading(const bool isFading)
	{
		m_isFading = isFading;
	}

	bool ParticleComponent::GetIsFading() const
	{
		return m_isFading;
	}

	std::string ParticleComponent::Serialize()
	{
		std::ostringstream ss;
		return ss.str();
	}

	void ParticleComponent::Deserialize(std::string_view toDeserialize)
	{
	}

	void ParticleComponent::Update(const float dt)
	{
		m_lifetime -= dt;
	}

}