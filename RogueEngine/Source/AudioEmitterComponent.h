#pragma once
#include "BaseComponent.h"

namespace Rogue
{
	class AudioEmitterComponent
		: public BaseComponent
	{
		std::string m_id;
	public:
		AudioEmitterComponent() = default;

		void setID(const std::string& id);
		std::string getID() const;

		std::string Serialize();
		void Deserialize(std::string toDeserialize);
	};
}