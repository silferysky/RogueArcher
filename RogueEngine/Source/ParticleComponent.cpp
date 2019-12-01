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