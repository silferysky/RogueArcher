#include "AudioEmitterComponent.h"
#include <sstream>

namespace Rogue
{
	void AudioEmitterComponent::setID(const std::string& id)
	{
		m_id = id;
	}

	std::string AudioEmitterComponent::getID() const
	{
		return m_id;
	}

	std::string AudioEmitterComponent::Serialize()
	{
		std::ostringstream ss;
		ss << m_id << ";";
		return ss.str();
	}

	void AudioEmitterComponent::Deserialize(std::string toDeserialize)
	{
	}
}