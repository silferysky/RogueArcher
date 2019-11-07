#pragma once
#include <string>

namespace Rogue
{
	class ISerializable
	{
	public:
		virtual std::string Serialize() = 0;
		virtual void Deserialize(std::string deserializeStr) = 0;
	};
}