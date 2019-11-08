#pragma once
#include <string>

namespace Rogue
{
	class ISerializable
	{
	public:
		virtual std::string Serialize() = 0;
		virtual void Deserialize(const std::string& deserializeStr) = 0;
		virtual void DisplayOnInspector() {};
	};
}